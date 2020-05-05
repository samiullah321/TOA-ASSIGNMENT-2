/*

Project: English Language Statement Parser

Made by:
1- Murad Popattia (17K-3722)
2- Muhammad Samiullah (17K-3745)

Follows the following grammar:

S -> NP VP
NP -> Pronoun | Proper_Noun | Det Nominal
Nominal -> Noun Nominal | Noun
VP -> Verb | Verb NP | Verb NP PP | Verb PP
PP -> Preposition NP

There are no mentions of adjectives or conjunctions and hence their files are kept empty

ALGORITHM FOLLOWED:

-> Two stacks are maintained. One in which the whole string is input. The second stack in which we push the productions of the cfg
-> Consecutively the words are popped from the input stack and checked with the top of the production stack. Based on the element placed on the top of the production stack,	
   the respective non terminal such as NP,VP,S,PP,Det and Nom is pushed onto the production stack
-> The conditions are made based on empirical data gathered from testing multiple test cases

*/

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#define BUFFSIZE 10000 /*make sure this is larger then the longest line...*/
using namespace std;

//we store all the nouns, verbs and so on into the map data structure which is just like the vector class. These are numbered in order the signify the order in which they are checked
map<int,string> noun; //1
map<int,string> verb; //2
map<int,string> adj; //3
map<int,string> pronoun; //4
map<int,string> prop_noun; //5
map<int,string> det; //6
map<int,string> prep; //7
map<int,string> conj; //8

fstream obj; //this is a global fstream obj in order to accomdate the output from file

bool flag[9]={false,true,true,true,true,true,true,true,true}; // flag array is used inorde to block or allow for checking the presence of type such as noun,verb etc
bool valid=true; //this is to make sure at the end the string input is valid or not

template <class t>
class stack{ //this is the stack class used in solving solve the following CFG provided
	
	private:
		t*a;
		int top;
		int bottom;
		int capacity;
		
	public:
		stack(int size){
			a = new t[size];
			top=-1;
			bottom=0;
			capacity=size;
		}
		
		void push(t word){
			if(top!=capacity){
				top++;
				a[top]=word;
			}	
			else cout<<"Stack full";
		}
		
		t pop(){ //this provides the last element in the stack which is technically popped first
			t element;
			if(top!=-1){
				element=a[top];
				top--;
				return element;
			}
			else{
				return "NULL";
			}
			
		}
		
		void popall(){
			while(top!=-1){
				pop();
			}
		}	
		
		t getbottomelement(){ //this function provides the first element in the stack
			return a[bottom];
		}
		
		t getelement(int index){
			if(index>=0)
			return a[index];
			else return "NULL";
		}
		
		int gettop(){
			return top;
		}
		
		t gettopelement(){
			return a[top];
		}
		
		int getsize(){
			return top;
		}
		
		void printstack(){
			
			if(top==-1)
			return;
			
			for(int i=0;i<=top;i++){
				obj<<a[i]<<" ";
			}
		}
		
};

//as we are dealing with stack our initial output is the reverse of the output shown and hence we use this function in order to bring out the desired output
bool reverse_file(const char* input, const char* output)  
{
    streamsize count=0;
    streamoff size=0,pos;
    char buff[BUFFSIZE];

    ifstream fin(input);
    ofstream fout(output);

    if(fin.fail() || fout.fail()){
        return false;
    }

    fin.seekg(0, ios::end);
    size = fin.tellg();
    fin.seekg(0);
    while(!fin.eof()){  
        fin.read(buff, BUFFSIZE);
        count = fin.gcount();
        reverse(buff,buff+count);
        pos = fin.tellg();
        if(pos<0) {
            pos = size;
        }
        fout.seekp(size - pos);
        fout.write(buff,count);
    }
    return true;
}

bool reverse_file_lines(const char* input, const char* output)
{
    streamsize count=0;

    char buff[BUFFSIZE];

    ifstream fin(input);
    ofstream fout(output);

    if(fin.fail() || fout.fail()){
        return false;
    }

    while(!fin.eof()){  
        fin.getline(buff, BUFFSIZE);
    /*if BUFFSIZE is smallest then line size gcount will return 0, 
        but we didn't handle it...*/
        count = fin.gcount();
        if(buff[count-1]==0)count--;
        reverse(buff,buff+count);
        fout.write(buff,count);
        if(!fin.eof()){
            fout<<endl;
        }
    }
    return true;
}

//this function opens the files of nouns,verbs etc. and reads from the files and inputs it to the map data structure.
void inputtomap(){
	
	string s;
	int i=0;
	fstream obj;
	obj.open("nouns.txt",ios::in);
	
	if(!obj)
	cout<<"unable to open file";	

	else{
		while(!obj.eof()){
			getline(obj,s);
			noun[i]=s;
			i++;
		}
	}
	
	i=0;
	obj.close();
	
	obj.open("pronouns.txt",ios::in);
	
	if(!obj)
	cout<<"unable to open file";	

	else{
		while(!obj.eof()){
			getline(obj,s);
			pronoun[i]=s;
			i++;
		}
	}
	
	i=0;
	obj.close();
	
	obj.open("adjectives.txt",ios::in);
	
	if(!obj)
	cout<<"unable to open file";	
	
	else{
		while(!obj.eof()){
			getline(obj,s);
			adj[i]=s;
			i++;
		}
	}
	
	i=0;
	obj.close();
	
	obj.open("conjunctions.txt",ios::in);
	
	if(!obj)
	cout<<"unable to open file";	

	else{
		while(!obj.eof()){
			getline(obj,s);
			conj[i]=s;
			i++;
		}
	}
	
	i=0;
	obj.close();
	
	obj.open("determiners.txt",ios::in);
	
	if(!obj)
	cout<<"unable to open file";	
	
	else{	
		while(!obj.eof()){
			getline(obj,s);
			det[i]=s;
			i++;
		}
	}
	
	i=0;
	obj.close();
	
	obj.open("proper_noun.txt",ios::in);
	
	if(!obj)
	cout<<"unable to open file";	
	
	else{
		while(!obj.eof()){
		obj>>s;
		prop_noun[i]=s;
		i++;
		}
	}
	
	i=0;
	obj.close();
	
	obj.open("prepositions.txt",ios::in);
	
	if(!obj)
	cout<<"unable to open file";
		
	else{
		while(!obj.eof()){
			getline(obj,s);
			prep[i]=s;
			i++;
		}
	}
	
	i=0;
	obj.close();
	
	obj.open("verbs.txt",ios::in);
	
	if(!obj)
	cout<<"unable to open file";
		
	else{
		while(!obj.eof()){
			getline(obj,s);
			verb[i]=s;
			i++;
		}
	}
	
	i=0;
	obj.close();
	
}

//this function is used to search for the element in the lists following a specific search order based on the way the types are numbered as shown in the above comments
//if the letter is found in one of the types then 1 is returned
int search(string s,int type){
		
		switch(type){
			
			case 1:{
				for(int i=1;i<noun.size();i++){
					if(s==noun[i]){
					
						return 1; 
					}
				}
				return 0;
				break;
			}
			
			case 2:{		
				for(int i=1;i<verb.size();i++){
					if(s==verb[i]){
						
						return 1;
					}
				}
				return 0;
				break;
			}
			
			case 3:{
				for(int i=1;i<adj.size();i++){
					if(s==adj[i]){
						
						return 1;
					}
				}
				return 0;
				break;
			}
			
			case 4:{
				for(int i=1;i<pronoun.size();i++){
					if(s==pronoun[i]){
						
						return 1;
					}
				}
				return 0;
				break;
			}
			
			case 5:{
				for(int i=1;i<prop_noun.size();i++){
					if(s==prop_noun[i]){
					
						return 1;
					}
				}
				return 0;
				break;
			}
			
			case 6:{
				for(int i=1;i<det.size();i++){
					if(s==det[i]){
						
						return 1;
					}
				}
				return 0;
				break;
			}
			
			case 7:{
				for(int i=1;i<prep.size();i++){
					if(s==prep[i]){
						
						return 1;
					}
				}
				return 0;
				break;
			}
			
			case 8:{
				for(int i=1;i<conj.size();i++){
					if(s==conj[i]){
						
						return 1;
					}
				}
				return 0;
				break;
			}
			
			default:{
				cout<<"Invalid type entered";
				return -1;
				break;
			}
		}	
	
}

int parser(string s,int space){ //THE MAIN FUNCTIONALITY

	stack<string> input(space+1); //stack for the input string
	stack<string> prod(space+1); //stack for the productions
	string *arr; //this array is used to accomadate the seperated from the string stream
	arr=new string[space+1]; //the sentence has space + 1 words
	string seperate; //this is to accodomate the seperate words
	int i,print=space+1; 
	bool indicator[space+1][8]; //this tells the nature of the word that it is noun + pronoun + verb etc
		
	//in order to get all the words seperated into an array
	stringstream ssin(s);
    while (ssin.good() && i < space+1){
        ssin >> arr[i];
        ++i;
    }
    
    //the seperate words are then pushed into the input stack
    for(i = 0; i < space+1; i++){
    	seperate=arr[i];
        input.push(seperate);
    }
    
    //the indicator array is initialized
    for(int i=0;i<space+1;i++){ 
		for(int j=1;j<=8;j++){
			indicator[i][j]=false;
		}
	}
	
	//the array is filled with the respective words. This is done as an alternate to the back track approach
	for(int i=0;i<space+1;i++){
		for(int j=1;j<=8;j++){
			if(search(input.getelement(i),j)){
				indicator[i][j]=true;
			}
			else indicator[i][j]=false;
		}
	}
	
    cout<<endl;
    string checker; //this is the checker string in which the popped word from stack is placed and then compared with the 
    
    for(i = space; i > -1; i--){ //this looped backwards as we read the sentence in backward manner
    	
    	checker=input.pop(); //the input is popped into this string

		if(search(checker,1) && flag[1]==true){ //**THE NOUN**// the word is a noun
			
			for(int d=0;d<9;d++){
				flag[d]=true; //all flags are made true to avoid unecessary check block of any type
			}
			
			//a word is mostly a noun + verb + pronoun and hence we check the condition for the differentiation
			if(prod.gettop()==-1 && !indicator[i][2] && !indicator[i][4]){ //the word is purely a noun and neither a pronoun nor a verb and nothing on top of stack
			
				prod.push("Nom"); // Nominal -> Noun Nominal | Noun
				
				obj<<endl;
				input.printstack(); 
				prod.printstack();
				
			}
			
			else if(prod.gettop()==-1 && indicator[i][2] && !indicator[i][4]){ //the word can be a verb and nothing on top of stack
					
				bool verb_present=false; //this is to find that whether there is a verb preceeding to it
				for(int f=0;f<i;f++){
					if(indicator[f][2]){
						verb_present=true; //atleast one verb found before the word
						break;
					}
				}
				
				//the word has no verb before it and the word right before it is not a determiner so block noun and next it searches for being a verb
				if(!verb_present && !indicator[i-1][6]){ 
					input.push(checker);
					flag[1]=false;
					indicator[i][1]=false;
					i++;
				}
				
				else if(!verb_present){ //the word has no verb preceeding to it and there might be a determiner before it then consider it as a verb and
					input.push(checker);
					flag[1]=false;
					indicator[i][1]=false;
					i++;
				}
								
				else{ //if there is a verb preceeding to it then consider it a noun otherwise verb next to a verb is invalid
						
					bool det_present=false; //search for the presence of a determiner
					bool prep_present=false; //search for the presence of a preposition
					for(int f=0;f<i;f++){
							if(indicator[f][6]){
								det_present=true;
								break;
							}
					}
					
					for(int f=0;f<i;f++){
							if(indicator[f][7]){
								prep_present=true;
								break;
							}
					}
					
					if(i-1>-1 && indicator[i-1][1]){ 
						
						//det noun* verb so NP BP
						if(det_present && !prep_present){ // making sure that preposition is not present before it else the prediction could be Prep Det Nom and hence could become PP
						
							input.push(checker);
							flag[1]=false;
							indicator[i][1]=false;
							i++;
						}
						
						else if(det_present){ //else if there is atleast a determiner present the possibility of det noun* so becoming NP and prep_present so prep NP -> PP
							
							prod.push("Nom");
							
							obj<<endl;
							input.printstack();
							prod.printstack();
							indicator[i][2]=false;
							
						}
						
					}
					else{ //else if preposition and determiner present then consider it as a Nom
											
						prod.push("Nom");
						
						obj<<endl;
						input.printstack();
						prod.printstack();
						indicator[i][2]=false;
					}
				}
				
			}
			
			else if(prod.gettop()==-1 && indicator[i][4]){ //if word is noun + pronoun 
			
				if(indicator[i-1][2]){ //and verb before it
					input.push(checker);
					flag[1]=false;
					indicator[i][1]=false;
					i++;
				}
								
				else{ //as the word has the tendency to become a pronoun then we can check block noun and verb
					
					input.push(checker);
					flag[1]=false;
					flag[2]=false;
					indicator[i][1]=false;
					indicator[i][2]=false;
					i++;
						
				}
			}
			
			else{ //there is something on top of the 2nd stack
								
				if(prod.gettopelement()=="Nom" && !indicator[i][2]){ //if the word is purely a noun
				
					obj<<endl;
					input.printstack();
					obj<<"Noun ";
					prod.printstack();
					
					obj<<endl;
					input.printstack();
					prod.printstack();
				
				}
				
				else if(prod.gettopelement()=="Nom" && indicator[i][2]){ //if the word can be a verb and top of stack is Nom
					bool verb_present=false;
						
					for(int f=0;f<i;f++){ //check if there is any verb preceeding to it
						if(indicator[f][2]){
							verb_present=true;
							break;
						}
					}
					
					if(!verb_present){ //consider the letter as a verb
					
						input.push(checker);
						flag[1]=false;
						indicator[i][1]=false;
						i++;
					}
					
					else if(verb_present){ //if there is a verb present then we can consider this as a noun
						
						obj<<endl;
						input.printstack();
						obj<<"Noun ";
						prod.printstack();
						
						obj<<endl;
						input.printstack();
						prod.printstack();
					}
					
					else {
						valid = false;
						break;
					}
					
				}
				
				else if((prod.gettopelement()=="NP")){ //the word can also be a verb and top of stack is NP then we know Noun NP is invalid so consider it a verb
					
					if(indicator[i][2] || indicator[i][7]){ //word can be a verb or a preposition. In any case just lock the noun check
						input.push(checker);
						flag[1]=false;
						indicator[i][1]=false;
						i++;
					}
					
					else { //else if the word cannot be a verb or preposition then the structure is invalid
						valid = false;
						break;
					}
					
				}
				
				else if(prod.gettopelement()=="VP"){ // if the top of the stack is VP
						
						bool det_present=false;
							for(int f=0;f<i;f++){
								if(indicator[f][6]){
									det_present=true;
									break;
							}
						}
						
						if(det_present && prod.getbottomelement()!="NP"){ //if there is a determiner present before it and following then noun there isn't an NP 
						
							obj<<endl;
							input.printstack();
							obj<<"Noun ";
							prod.printstack();
							
							prod.popall();
							prod.push("Nom"); //as there can be case of Det Nom VP
							prod.push("VP");
							
							obj<<endl;
							input.printstack();
							prod.printstack();
						
						}
						
						else if(indicator[i][4]){ //if the word has the tendency to become a pronoun then it can become one if the above conditions dont hold true
							input.push(checker);
							flag[1]=false;
							flag[2]=false;
							indicator[i][1]=false;
							indicator[i][2]=false;
							i++;
						}
						
						else{
							valid=false;
							break;
						}	
				}
				
				else if(prod.gettopelement()=="PP"){ //if there is a PP on top of the stack
						
						bool det_present=false;
						for(int f=0;f<i;f++){
								if(indicator[f][6]){
									det_present=true;
									break;
							}
						}
						
					if(det_present && prod.getbottomelement()!="NP"){ //same condition as with VP above
							
							obj<<endl;
							input.printstack();
							obj<<"Noun ";
							prod.printstack();
							
							prod.popall();
							prod.push("Nom"); //as there can be a case of Det Nom PP
							prod.push("PP");
							
							obj<<endl;
							input.printstack();
							prod.printstack();
					}
					
					else if(indicator[i][2] && prod.gettopelement()=="PP"){ // if the word is searched as a noun but has the tendency to become a verb so case of Verb PP
							input.push(checker);
							flag[1]=false;
							indicator[i][1]=false;
							i++;
					}
					
					else{
						valid=false;
						break;
					}
			
				}
			}
    	}
    	
		else if(search(checker,2) && flag[2]==true){ //word is checked as a verb
    			
    		for(int d=0;d<9;d++){
				flag[d]=true;
			}
    		
    		if(prod.gettop()==-1){ //if the first element checked is a verb then VP as NP VP is a valid production
					obj<<endl;
					input.printstack();
					obj<<"VERB ";
					prod.printstack();
					
					prod.push("VP");
					
					obj<<endl;
					input.printstack();
					prod.printstack();
			}
			
			else{
			
				if(prod.gettopelement()=="NP"){
					//if the word can be a preposition then check for it first as PP -> Prep NP and then VERB PP can become VP hence we check for the smaller productions first
					if(indicator[i][7]){ 
						input.push(checker);
						flag[2]=false;
						indicator[i][2]=false;
						i++;
					}	
					
					else{				
					obj<<endl;
					input.printstack();
					obj<<"VERB ";
					prod.printstack();
					
					prod.pop();
					prod.push("VP"); //VP -> VERB
					
					obj<<endl;
					input.printstack();
					prod.printstack();
					}
					
				}
				
				else if(prod.gettopelement()=="PP"){ //if the top of the production stack lies PP
						
							if(prod.getbottomelement()=="NP"){ //if NP PP
								obj<<endl;
								input.printstack();
								obj<<"VERB ";
								prod.printstack();
								
								prod.popall();
								prod.push("VP"); //VERB NP PP -> VP
								
								obj<<endl;
								input.printstack();
								prod.printstack();
							}
							
							else{
								obj<<endl;
								input.printstack();
								obj<<"VERB ";
								prod.printstack();
								
								prod.pop();
								prod.push("VP"); // VERB PP -> VP
								
								obj<<endl;
								input.printstack();
								prod.printstack();
							}
							
				}
									
				else{
					valid=false;
					break;	
				}
			}
		}
		
//		else if(search(checker,3) && flag[3]==true){
//			
//			for(int d=0;d<9;d++){
//				flag[d]=true;
//			}
//			
//		}
		
		else if(search(checker,4) && flag[4]==true){
			
			for(int d=0;d<9;d++){
				flag[d]=true;
			}
			
    		if(prod.gettop()==-1){ //as there can be a VERB before it and hence NP VERB NP can be a vlid production
				obj<<endl;
				input.printstack();
				obj<<"PRO ";
				prod.printstack();
				
				prod.push("NP");
				
				obj<<endl;
				input.printstack();
				prod.printstack();				
			}
			
			else{
				
				if(prod.gettopelement()=="VP"){ //the top of stack is VP
					
					if(prod.getbottomelement()=="Nom"){ //if the word has a capability to become a determiner and being a pronoun would give incorrect result
						input.push(checker);
						flag[4]=false;
						indicator[i][4]=false;
						i++;
					}
					
					else{
						
					obj<<endl;
					input.printstack();
					obj<<"PRO ";
					prod.printstack();
					
					prod.pop();
					prod.push("NP"); //PRO VERB -> NP VP
					prod.push("VP");
					
					obj<<endl;
					input.printstack();
					prod.printstack();
					
					}
				}
				
				else if(prod.gettopelement()=="PP"){
					obj<<endl;
					input.printstack();
					obj<<"PRO ";
					prod.printstack();
					
					prod.pop();
					prod.push("NP"); //then the prediction PRO PP -> NP PP
					prod.push("PP");
					
					obj<<endl;
					input.printstack();
					prod.printstack();
				}
				
				else {
					valid=false;
					break;
				}	
			}
		}
		
		else if(search(checker,5) && flag[5]==true){ //same as the pronoun
			
			for(int d=0;d<9;d++){
				flag[d]=true;
			}
			
    		if(prod.gettop()==-1){ 
				obj<<endl;
				input.printstack();
				obj<<"PROP_NOUN ";
				prod.printstack();
				
				prod.push("NP");
				
				obj<<endl;
				input.printstack();
				prod.printstack();				
			}
			
			else{
				
				if(prod.gettopelement()=="VP"){
					
					obj<<endl;
					input.printstack();
					obj<<"PROP_NOUN ";
					prod.printstack();
					
					prod.pop();
					prod.push("NP");
					prod.push("VP");
					
					obj<<endl;
					input.printstack();
					prod.printstack();
					
				}
				
				else if(prod.gettopelement()=="PP"){
					obj<<endl;
					input.printstack();
					obj<<"PROP_NOUN ";
					prod.printstack();
					
					prod.pop();
					prod.push("NP");
					prod.push("PP");
					
					obj<<endl;
					input.printstack();
					prod.printstack();
				}
				
				else {
					valid=false;
					break;
				}	
			}
			
		}
		
		else if(search(checker,6) && flag[6]==true){ //DETERMINER
			
			for(int d=0;d<9;d++){
				flag[d]=true;
			}
			
    		if(prod.gettop()==-1){ //if sentence end with determiner then it is invalid as there must be nominal first
				valid=false;
				continue;
			}
    		
    		else{
				if(prod.gettopelement()=="Nom"){
					obj<<endl;
					input.printstack();
					obj<<"Det ";
					prod.printstack();
					
					prod.pop();
					prod.push("NP"); //Det Nom -> NP
					
					obj<<endl;
					input.printstack();
					prod.printstack();
				}
				else if(prod.gettopelement()=="VP" && prod.getbottomelement()=="Nom"){
					obj<<endl;
					input.printstack();
					obj<<"Det ";
					prod.printstack();
					
					prod.popall();
					prod.push("NP"); //if the stack is like VP Nom then Det Nom VP -> NP VP
					prod.push("VP");
					
					obj<<endl;
					input.printstack();
					prod.printstack();
				}
				else if(prod.gettopelement()=="PP" && prod.getbottomelement()=="Nom"){
					obj<<endl;
					input.printstack();
					obj<<"Det ";
					prod.printstack();
					
					prod.popall();
					prod.push("NP"); //if the stack is like PP Nom then Det Nom PP can become NP PP
					prod.push("PP");
					
					obj<<endl;
					input.printstack();
					prod.printstack();
				}
				else{
					valid=false;
					break;	
				}
			}
		}
		
		else if(search(checker,7) && flag[7]==true){ //if the word is a preposition
			
			for(int d=0;d<9;d++){
				flag[d]=true;
			}
			
			if(prod.gettop()==-1){ //the sentence cannot end with
				valid=false;
				break;
			}
			
			else{
				
				if(prod.gettopelement()=="NP"){ 
					obj<<endl;
					input.printstack();
					obj<<"Prep ";
					prod.printstack();
					
					prod.pop();
					prod.push("PP"); //Prep NP -> PP only valid prediction
					
					obj<<endl;
					input.printstack();
					prod.printstack();
				}
				
				else{
					valid=false;
					break;
				}
				
			}
			
		}
		
//		else if(search(checker,8) && flag[8]==true){
//			
//			for(int d=0;d<9;d++){
//				flag[d]=true;
//			}
//			
//		} 	
		
		else{
			valid=false;
			break;
		}	
    }
	
	/*
	Validation conditions:
	
	1- two elements on production stack
	2- stack should be like VP NP and in array form like NP VP
	3- all input stack should be consumed
	4- the string still holds valid
	
	*/
	
	if(prod.getsize()==1 && prod.gettopelement()=="VP" && prod.getbottomelement()=="NP" && input.getsize()==-1 && valid){ 
		prod.popall();
		obj<<endl;
		prod.push("S");
		prod.printstack();
		obj<<endl;
	}
	
	else{
		valid=false;
	}
	
	if(valid){ //if valid then reverse the lines of the output file and overwrite it in reverse form
		reverse_file("output.txt", "test.tmp");
    	reverse_file_lines("test.tmp","output.txt");
    	
    	fstream obj1;
    	obj1.open("output.txt",ios::in); //the display the file data onto the console
    	
    	string s;
    	while(!obj1.eof()){
    		getline(obj1,s);
    		cout<<s<<endl;
		}
		
		obj.close();
    	
	}
	else{
		//else display incorrect structure
		obj.close();
		fstream obj1;
		obj1.open("output.txt",ios::out|ios::trunc);
		obj1<<"Incorrect Structure";
		obj1.close();
	
    	obj1.open("output.txt",ios::in);
    	
    	string s;
    	while(!obj1.eof()){
    		getline(obj1,s);
    		cout<<s;
	}
	
	}
	return 0;
	
}

int main(void){
	
	string s;
	inputtomap();
	char c;
	int i=0,space=0;
	
	obj.open("input.txt",ios::in); //this is file from where we read the input
	getline(obj,s);
	obj.close();
	
	//** in order to know the number of words in a sentence
	while(i<=s.size()){
		c=s[i++];
		if(c==' ')
		space++;
	}
	
	obj.open("output.txt",ios::out);
	
	//send the string to parse
	parser(s,space);	
	
	//clear all the map objects
	noun.clear();
	verb.clear();
	pronoun.clear();
	adj.clear();
	prep.clear();
	conj.clear();
	det.clear();
	prop_noun.clear();
	
	return 0;
}
