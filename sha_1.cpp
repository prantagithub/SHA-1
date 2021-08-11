#include <iostream>
#include <cstring>
#include <vector>
#include <bitset>
#include <sstream>
#define LFTROT(word,bit) ((word<<bit) | (word>>(32-bit)))
#define ul unsigned long

using namespace std;
//---------------pre-processing-------------------

vector<ul> convert_to_binary(const string message){
	vector<ul> block;
	for(int i=0;i<message.length();i++){
		bitset<8> message_char(message[i]);
		block.push_back(message_char.to_ulong());
	}
	return block;
}
/*
	convert_to_binary converts each of the message character into a corresponding binary code
*/

void message_padding(vector<ul>& block){
	int l = block.size();
	int k = 447-l;
	ul padd_1 = 0x80; //adds 10000000
	block.push_back(padd_1);
	k -=7; // since 7 zeros have been added
	for(int i=0;i<k/8;i++){
		ul t = 0x00000000;
		block.push_back(t);
	}

	bitset<64> size_bits(l);
	string size_bits_string = size_bits.to_string();
	//dividing into 8 bit words
	for(int i=0;i<64;i+=8){
		bitset<8> temp(size_bits_string.substr(i,8));
		block.push_back(temp.to_ulong());
	}
	//padding done
}

/*
	Padding to add padding bits ad size bits to the processed message
*/

vector<ul> resize_block(vector<ul> block){
	vector<ul> res;
	for(int i=0;i<64;i+=4){
		bitset<32> temp(0);
		temp |= (ul)block[i]<<24;
		temp |= (ul)block[i+1]<<16;
		temp |= (ul)block[i+2]<<8;
		temp |= (ul)block[i+3];
		res.push_back(temp.to_ulong());
	}
	for(int i=16;i<80;i++){
		bitset<32> temp(0),t1(res[i-3]),t2(res[i-8]),t3(res[i-14]),t4(res[i-16]);
		temp |= (t1^t2^t3^t4);
		temp = LFTROT(temp,1);
		res.push_back(temp.to_ulong());
	}
	return res;
}

/*
resizing for all the 80 stages of hashing
*/
//------------------Hash-----------------------
void hashing_handler(vector<ul>& hash,vector<ul> block,vector<ul> key){
	ul N = 4294967296,k=0;
	ul a=hash[0],b=hash[1],c=hash[2],d=hash[3],e=hash[4];
	//stage 1-----------------------
	for(int i=0;i<20;i++){
		k=key[0];
		ul f = (b & c) | (~b & d);
		ul temp = (LFTROT(a,5)+f+e+k+block[i])%N;
		e = d;
		d = c;
		c = LFTROT(b,30);
		b = a;
		a = temp;
		hash[0]=(hash[0]+a)%N;
		hash[1]=(hash[1]+a)%N;
		hash[2]=(hash[2]+a)%N;
		hash[3]=(hash[3]+a)%N;
		hash[4]=(hash[4]+a)%N;
	}
	//stage 2------------------------
	for(int i=20;i<40;i++){
		k=key[1];
		ul f = (b^c^d);
		ul temp = (LFTROT(a,5)+f+e+k+block[i])%N;
		e = d;
		d = c;
		c = LFTROT(b,30);
		b = a;
		a = temp;
		hash[0]=(hash[0]+a)%N;
		hash[1]=(hash[1]+a)%N;
		hash[2]=(hash[2]+a)%N;
		hash[3]=(hash[3]+a)%N;
		hash[4]=(hash[4]+a)%N;
	}
	//stage 3
	for(int i=40;i<60;i++){
		k=key[2];
		ul f = (b & c) | (b & d) | (c & d);
		ul temp = (LFTROT(a,5)+f+e+k+block[i])%N;
		e = d;
		d = c;
		c = LFTROT(b,30);
		b = a;
		a = temp;
		hash[0]=(hash[0]+a)%N;
		hash[1]=(hash[1]+a)%N;
		hash[2]=(hash[2]+a)%N;
		hash[3]=(hash[3]+a)%N;
		hash[4]=(hash[4]+a)%N;
	}
	//stage 4
	for(int i=60;i<80;i++){
		k=key[3];
		ul f = (b^c^d);
		ul temp = (LFTROT(a,5)+f+e+k+block[i])%N;
		e = d;
		d = c;
		c = LFTROT(b,30);
		b = a;
		a = temp;
		hash[0]=(hash[0]+a)%N;
		hash[1]=(hash[1]+a)%N;
		hash[2]=(hash[2]+a)%N;
		hash[3]=(hash[3]+a)%N;
		hash[4]=(hash[4]+a)%N;
	}
}
//---------------output printer-------------
string print_output(vector<ul> hash){
	string res="";
	for(int i=0;i<hash.size();i++){
		stringstream stream;
		stream<<std::hex<<hash[i];
		string t;
		stream>>t;
		res+=t+" ";
	}
	return res;
}
//----------driver function--------------
string SHA_1(const string message){
	vector<ul> block;
	vector<ul> hash = {0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,0xC3D2E1F0};
	vector<ul> key = {0x5A827999,0x6ED9EBA1,0x8F1BBCDC,0xCA62C1D6};
	block = convert_to_binary(message);
	message_padding(block);
	block = resize_block(block);
	hashing_handler(hash,block,key);
	return print_output(hash);
}

//-----------------------------------------

int main(){
	string message = "The quick brown fox jumps over the lazy cog";
	cout<<SHA_1(message)<<endl;
}