/*
Multiplication in between any two is represented as *
Inverse functions have prefix ' arc ' infornt of sin,cos,...
Input file is "input.txt"
*/

#include<iostream>
#include<fstream>
#include<string>

using namespace std;

string diff(string s); // function initialised first and defined later

struct term{  // structure for a function, var stores the function, negative stores the sign
	string var;   // of the function, uvrule stores whether the function includes *,/
	string coeff;
	int negative;
	int uvrule;

	term *next;
	term *prev;

	term(){
		var = "";
		coeff = "";
		negative = 0;
		uvrule = 0;
	}
};

class list{ // list for connecting all terms of the given input
private:
	term* head;
	term* tail;
public:
	list(){
		head = NULL;
		tail = NULL;
	}

	void addend(string var,string coeff,int negative,int uvrule);
	void display();
	term* topterm();                                
};

void list::addend(string var,string coeff,int negative,int uvrule){
	term* newterm = new term;
	newterm->var = var;
	newterm->coeff = coeff;
	newterm->negative = negative;
	newterm->uvrule = uvrule;

	if(tail!=NULL){
		tail->next = newterm;
	}

	newterm->prev=tail;         // initially attaches new node after tail
	tail = newterm;         // tail becomes the last node once again
	tail->next = NULL;

	if(head == NULL){  // the first instance where the head and tail separate out
		head=newterm;
	}
}

void list::display(){
	term* current = head;
	while(current != NULL)
	{
		cout<<current->var<<" "<<current->coeff<<" "<<current->negative<<" "<<current->uvrule<<"\n";
		current = current->next;
	}
}

term* list::topterm(){
	return head;
}

string inttostring(int n){ // function to convert int to string
	if(n == 0) return "0";
	string s = "";
	char c;
	int x;
	while(n > 0){
		x = n%10;
		c = '0'+x;
		s = c + s;
		n /= 10;
	}

	if(n < 0){
		n = n*(-1);
		while(n > 0){
			x = n%10;
			c = '0'+x;
			s = c + s;
			n /= 10;
		}
		s = '-' + s;
	}

	return s;
}

int checkpower(string s){
	for(int i=0;i<s.length();i++){
		if(s[i] == '^') return 1;
	}
	return 0;
}

string remove_brackets(string s){
	if(s[0]=='(' && s[s.length()-1]==')'){
		return s.substr(1,s.length()-2);
	}

	return s;
}

string differnetiation(string s){
	if(s.length() == 1){
		if(s == "x"){
			return "1";
		}
		else return "0";
	}

	string num = "";
	for(int k=0;k<s.length();k++){  // for functions like 2x ( or simply kx)
		if(s[k]>=48 && s[k]<=57){
			num += s[k];
		}
		else{
			s = s.substr(k);
			break;
		}
	}
	if(num != ""){
		return num + "*(" + diff(s) + ")";
	}

	int open = 0;
	int k = 0;
	for(k=0;k<s.length();k++){
		if(s[k] == '('){
			open += 1;
		}
		else if(s[k] == ')'){
			open -= 1;
		}

		if(s[k]=='^' && open==0){
			break;
		}
	}
	if(s[k]=='^' && open==0){
		string u,v;
		u = s.substr(0,k);
		u = remove_brackets(u);
		v = s.substr(k+1);
		v = remove_brackets(v);

		int x_u=0,x_v=0;
		for(int j=0;j<u.length();j++){
			if(u[j] == 'x'){
				x_u = 1;
				break;
			}
		}
		for(int j=0;j<v.length();j++){
			if(v[j] == 'x'){
				x_v = 1;
				break;
			}
		}

		if(x_u==1 && x_v==0){    //f(x)^(n)
			int n = 0;
			for(int j=0;j<v.length();j++){
				if(v[j]>=48 && v[j]<=57){
					n = n*10 + (v[j]-'0');
				}
			}
			if(n == 0) return "0";
			if(n == 1) return "1";  
			return inttostring(n) + "*(" + u + ")^(" + inttostring(n-1) + ")*(" + diff(u) + ")";
		}
		else if(x_u==0 && x_v==1){  // e^f(x)  and  b^f(x) (constant b)
			if(u == "e"){
				return s + "*(" + diff(v) + ")";
			}
			else{
				return s + "*ln(" + u + ")*(" + diff(v) + ")";
			}
		}
		else{  // f(x)^g(x)
			return s + "*(ln("+u+")*("+diff(v) + ")+"+"("+v+"/"+u+")*("+diff(u)+"))";
		}
	}
	else if(s[0] == 'l'){ // ln(f(x))
		string t = "";
		int i = 0;
		while (i<s.length() && s[i] != '('){ i++; }
		t = s.substr(i+1,s.length()-i-2);

		return "(1/(" + t + "))*(" + diff(t) + ")";
	}

	int i = 0;
	string t = "";
	while(i<s.length() && s[i] != '('){
		t += s[i];
		i++;
	}
	i++;
	open = 1;
	string x = "";
	while(i<s.length() && open!=0){
		if(s[i] == '('){
			open += 1;
		}
		else if(s[i] == ')'){
			open -= 1;
		}
		if(open != 0){
			x += s[i];
		}
		i++;
	}
	if(t.length() == 3){
		if(t == "sin"){
			return "cos(" + x + ")*(" + diff(x) + ")";
		}
		else if(t == "cos"){
			return "-sin(" + x + ")*(" + diff(x) + ")";
		}
		else if(t == "tan"){
			return "sec(" + x + ")^(2)*(" + diff(x) + ")";
		}
		else if(t == "sec"){
			return "sec(" + x + ")*tan(" + x + ")*(" + diff(x) + ")";
		}
		else if(t == "csc"){
			return "-csc(" + x + ")*cot(" + x + ")*(" + diff(x) + ")";
		}
		else if(t == "cot"){
			return "-(csc(" + x + "))^(2)*(" + diff(x) + ")";
		}
	}
	else if(t.length() == 4){
		if(t == "sinh"){
			return "cosh(" + x + ")*(" + diff(x) + ")";
		}
		else if(t == "cosh"){
			return "sinh(" + x + ")*(" + diff(x) + ")";
		}
		else if(t == "tanh"){
			return "1 - (tanh(" + x + "))^(2)*(" + diff(x) + ")";
		}
		else if(t == "sech"){
			return "-tanh(" + x + ")*sech(" + x + ")*(" + diff(x) + ")";
		}
		else if(t == "csch"){
			return "-csch(" + x + ")*coth(" + x + ")*(" + diff(x) + ")";
		}
		else if(t == "coth"){
			return "1 - (coth(" + x + "))^(2)*(" + diff(x) + ")";
		}
	}
	else if(t.length() == 6){
		if(t == "arcsin"){
			return "1/sqrt(1-(" + x + ")^(2))*(" + diff(x) + ")";
		}
		else if(t == "arccos"){
			return "-1/sqrt(1-(" + x + ")^(2))*(" + diff(x) + ")";
		}
		else if(t == "arctan"){
			return "1/(1+(" + x + ")^(2))*(" + diff(x) + ")";
		}
		else if(t == "arcsec"){
			return "1/(|(" + x + ")|*sqrt((" + x + ")^(2)-1))*(" + diff(x) + ")";
		}
		else if(t == "arccsc"){
			return "-1/(|(" + x + ")|*sqrt((" + x + ")^(2)-1))*(" + diff(x) + ")";
		}
		else if(t == "arccot"){
			return "-1/(1+(" + x + ")^(2))*(" + diff(x) + ")";
		}
	}

	return "";
}

string diff(string s){
	if(s == "") return s;
	// if(s == "") return;
	list l;
	int n = s.length();
	int open = 0;
	int close = 0;
	string temp = "";
	if(s[0] != '-'){
		temp += '+';
		temp += s[0];
		if(s[0] == '(') open += 1;
	}
	else temp += '-';
	for(int i=1;i<n;i++){        // Adding terms to the list.
		if(s[i] == '('){
			open += 1;
			temp += s[i];
		}
		else if(s[i] == ')'){
			open -= 1;
			temp += s[i];
			if(i+1<n && (s[i+1]=='+' || s[i+1]=='-') && open==0){
				l.addend(temp,"",0,0);
				temp = s[i+1];
				i++;
			}
			else if(i+1<n && s[i+1]=='^'){
				temp += "^(";
				i += 3;
				while(i<n && s[i]!=')'){
					temp += s[i];
					i++;
				}
				temp += s[i];
				if(i+1<n && (s[i+1]=='+' || s[i+1]=='-')){
					l.addend(temp,"",0,0);
					temp = s[i+1];
					i++;
				}
				else temp += s[i];
			}
		}
		else if(s[i]=='^'){
			temp += "^(";
			i += 2;
			while(i+1<n && s[i]!= ')'){
				temp += s[i];
				i++;
			}
			temp += s[i];
			if(i+1<n && (s[i+1]=='+' || s[i+1]=='-') && open==0){
				l.addend(temp,"",0,0);
				temp = s[i+1];
				i++;
			}
		}
		else if((s[i]=='+' || s[i]=='-') && open==0){
			l.addend(temp,"",0,0);
			temp = s[i];
		}
		else{
			temp += s[i];
		}
	}
	l.addend(temp,"",0,0);

	term *current = l.topterm();   //Knowing the sign(+ or -) of each term.
	while(current != NULL){
		temp = current->var;
		if(temp[0] == '+') current->negative = 0;
		else current->negative = 1;

		current->var = temp.substr(1);
		current = current->next;
	}

	//u*v u/v
	current = l.topterm();
	while(current != NULL){  // separating terms which have *,/
		temp = current->var;
		open = 0;
		for(int i=0;i<temp.length();i++){
			if(temp[i] == '('){
				open += 1;
			}
			else if(temp[i] == ')'){
				open -= 1;
			}
			else if(temp[i]=='*' && open==0){
				current->uvrule = 1;
			}
			else if(temp[i]=='/' && open==0){
				current->uvrule = 2;
			}
		}
		current = current->next;
	}

	current = l.topterm();
	while(current != NULL){
		if(current->uvrule == 0){
			current->var = remove_brackets(current->var);
			current->var = differnetiation(current->var);
			temp = current->var;
			if(temp[0] == '-'){
				if(current->negative == 1){
					current->negative = 0;
				}
				else{
					current->negative = 1;
				}
				current->var = temp.substr(1);
			}
		}
		if(current->uvrule == 1){    // u*v
			string t = current->var;
			string t1,t2;
			t1 = "";
			open = 0;
			for(int i=0;i<t.length();i++){
				if(t[i] == '('){
					open += 1;
					t1 += t[i];
				}
				else if(t[i] == ')'){
					open -= 1;
					t1 += t[i];
					if(t[i+1] == '*' && open==0){
						t2 = t.substr(i+2);
						break;
					}
				}
				else{
					t1 += t[i];
				}
			}
			t1 = remove_brackets(t1);
			t2 = remove_brackets(t2);
			current->var = "(" + t1 + ")*(" + diff(t2) + ") + ";
			current->var += "(" + t2 + ")*(" + diff(t1) +")";
		}
		if(current->uvrule == 2){    // u/v
			string t = current->var;
			string t1,t2;
			t1 = "";
			open = 0;
			for(int i=0;i<t.length();i++){
				if(t[i] == '('){
					open += 1;
					t1 += t[i];
				}
				else if(t[i] == ')'){
					open -= 1;
					t1 += t[i];
					if(t[i+1] == '/' && open==0){
						t2 = t.substr(i+2);
						break;
					}
				}
				else{
					t1 += t[i];
				}
			}
			t1 = remove_brackets(t1);
			t2 = remove_brackets(t2);
			current->var = "((" + diff(t1) + ")*(" + t2 + ") - ";
			current->var += "(" + diff(t2) + ")*(" + t1 + "))";
			current->var += "/(" + t2 + ")^(2)";
		}
		current = current->next;
	}

	string ans = "";
	current = l.topterm();
	if(current->negative == 1){
		ans += '-';
	}
	ans += current->var;
	current = current->next;
	while(current != NULL){
		if(current->negative == 1) ans += " - ";
		else ans += " + ";
		ans += current->var;
		current = current->next;
	}

	string t = "";
	for(int i=0;i<ans.length();i++){
		if(ans[i]=='*'){
			if(i+1<ans.length() && !(ans.substr(i+1,3)=="(1)" || ans.substr(i+1,3)=="(0)")){
				t += ans[i];
			}
			else i += 3;
		}
		else t += ans[i];
	}
	return t;
}

int main(){
	fstream file;

	file.open("input.txt");

	char c;
	string s = "";

	while(file.get(c)){
		if(c != ' '){
			s += c;
		}
	}

	string ans = diff(s);
	cout<<ans<<"\n";

	return 0;
}