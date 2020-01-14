/**********|**********|**********|
Program: TC01_A2_NG_YAN_TENG.cpp
Course: OOPDS
Year: 2018/19 Trimester 2
Name: Ng Yan Teng
ID: 1171303161
Lecture Section: TC01
Tutorial Section: TT02
Email: teng8111@gmail.com/1171303161@student.mmu.edu.my
Phone: 010-3333933
**********|**********|**********/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

typedef vector<string> vstring;

class Index
{
	private:
	vector<vstring> lineList;
	vector<vstring> wordList;
	vstring stopWords;
	int fileno;
	int lineno;
	
	public:
	void getStopWords();
	int binarySearch(string input);
	bool filterWords(string &input);
	void placeWords(string input, int line, int file);
	void readFile();
	void getInput();
	void returnSearch(string input);
};

void Index::getStopWords()
{
	ifstream sFile("StopWords.txt");
	
	while(!sFile.eof())	//writes all the words in StopWords.txt into a vector
	{
		copy(istream_iterator<string>(sFile),istream_iterator<string>(), back_inserter(stopWords));
	}
}

int Index::binarySearch(string input)
{
	int first=0, last=wordList.size()-1, middle;
		
	while (first <= last)
    {
        middle=(first+last)/2;	//this finds the mid point
        if (input==wordList.at(middle).at(0)) 
		{
            return middle;
        }
        else if (input<wordList.at(middle).at(0))	// if it's in the lower half
        {
            last=middle-1;
        }
        else 
		{
            first=middle+1;			//if it's in the upper half
        }
    }
    return -1;
}

bool Index::filterWords(string &input)	//determine if string passed is valid to be write into index vector
{
	
	for(int i=0;i<stopWords.size();i++)	//if string passed is in StopWords.txt, return false
	{
		if(input==stopWords[i])
			return false;
	}
	
	if(input.length()==1)	//if string passed is a single character or digit, return false
		return false;
		
	label:
	if(input.back()=='.'||input.back()==','||input.back()=='"'||
	input.back()==':'||input.back()=='-'||input.back()=='?'||input.back()==')') //removes unwanted symbols by checking last element of string
	{
		input.pop_back();
		goto label;	//double-check
	}
	else if(input.front()=='.'||input.front()==','||input.front()=='"'||	//removes unwanted symbols by checking first element of string
	input.front()=='-'||input.front()=='(')
	{
		input.erase(input.begin());
		goto label;
	}
	else if((input[input.length()-2])=='\'' && input.back()=='s')	//removes the 's behind word
	{
		input.pop_back();
		input.pop_back();
	}
	
	return true;
}

void Index::placeWords(string input, int line, int file) //place valid words into vector
{
	vstring tempvect;
	int searchResult=binarySearch(input);
	
	if(wordList.empty())	//when 2d index vector is empty, create new row of vector with the string, file no. and line no.
	{
		tempvect.push_back(input);
		tempvect.push_back(to_string(file));
		tempvect.push_back(to_string(line));
		wordList.push_back(tempvect);
		tempvect.clear();
	}
	else if(searchResult!=-1)	//if string passed already has existing row of vector, push_back file no. and line no. only
	{
		wordList.at(searchResult).push_back(to_string(file));
		wordList.at(searchResult).push_back(to_string(line));		
		tempvect.clear();
	}
	else	//if string passed does not have existing row of vector, create a new row of vector with string, file no. and line no.
	{
		tempvect.push_back(input);
		tempvect.push_back(to_string(file));
		tempvect.push_back(to_string(line));
		wordList.push_back(tempvect);
		tempvect.clear();
	}
	sort(wordList.begin(),wordList.end()); //sorts wordList by the first element of each word alphabetically
}

void Index::readFile()
{
	ifstream lFile;
	string line, word, test;
	vstring tempvect;
	
	fileno=1;
	lineno=1;
	
	for(int i=1;i<=10;i++) //loop to open 10 files
	{
		lineno=1;
		string filename=to_string(i)+".txt";
		lFile.open(filename);	
		
		if (!lFile) 	//if unable to open file, display error message					
		{													
			cerr << "Unable to open file.";		
			exit(1);							
		}
		while(true)
		{
			getline(lFile,line);		//fetchs the entire line and writes into a 2d vector 
			tempvect.push_back(line);
			transform(line.begin(), line.end(), line.begin(), ::tolower);	//entire string
			istringstream ss=istringstream(line);
			
			while(ss>>word)
			{	
				if(filterWords(word))				//determine if string passed is valid
					placeWords(word,lineno,fileno);	//if valid write into vector
			}
			if(lFile.eof())	//break when end-of-file
				break;
			lineno++;
		}
		lineList.push_back(tempvect);
		tempvect.clear();
		lFile.close();
		fileno++;
	}
}

void Index::getInput()
{
	string input;
	
	while(input!="-1")
	{
		cout<<"\nEnter word to be search, to exit enter -1: ";
		cin>>input;
		transform(input.begin(), input.end(), input.begin(), ::tolower);	//tolower input
		returnSearch(input);
	}
}

void Index::returnSearch(string input)
{
	bool invalidWord=true;	//indicator for input if it exist in index vector, true for invalid vice versa
	int searchResult;
	
	if(input=="-1")
		invalidWord=false;	//-1 to quit program, error message below will not be print
	
	searchResult=binarySearch(input);
	if(searchResult!=-1)
	{
		for(int j=1;j<wordList[searchResult].size();j+=2)
		{
			fileno=stoi(wordList[searchResult][j]);
			lineno=stoi(wordList[searchResult][j+1]);
				
			cout<<"\nWord \""<<input<<"\" can be found in "<<wordList[searchResult][j]<<".txt, line "
				<<wordList[searchResult][j+1]<<": \n"			//cout file name and line no.
				<<lineList[fileno-1][lineno-1]<<endl<<endl;		//cout line where input is to be found
		}
		invalidWord=false; //input can be found in index, set indicator to false
	}
	else if(invalidWord)
		cout<<"Word does not exist.\n";	//display error message if input does not exist in index
}

int main()
{
	Index a;
	
	a.getStopWords();
	a.readFile();
	a.getInput();
	
	return 0;
}