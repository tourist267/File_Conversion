

#include <iostream>
#include <vector>
#include <io.h>
#include <cstdlib>
#include <string>
#include <windows.h>
#include <fstream>
#include <direct.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <cctype>

using namespace std;



#define MAX_CHAR_NUM			100
/*
	将wchar_t的双字节数组转化成支持中文的string类型
*/
void Wchar_T_To_String(string& szDst, wchar_t* wchar)
{
	wchar_t* Wtext = wchar;

	DWORD dwnum = WideCharToMultiByte(CP_OEMCP,NULL,Wtext,-1,NULL,0,NULL,FALSE);

	char* psText;
	psText = new char[dwnum];

	WideCharToMultiByte(CP_OEMCP, NULL, Wtext, -1, psText, dwnum, NULL, FALSE);
	szDst = psText;
	delete[]psText;

}

/*
	获取当前文件的文件路径,文件名,文件类型
*/
void Get_Info(string& path,string& name,string& type)
{
	wchar_t Char[MAX_CHAR_NUM + 1];
	wchar_t Path[MAX_CHAR_NUM], Name[30], Type[5];
	int i, j = 0, k;


	HMODULE hModule = GetModuleHandle(NULL);
	//获取当前路径
	GetModuleFileName(hModule, Char, MAX_CHAR_NUM);


	//检测当前获取的路径+名称长度
	for (i = 0, j = 0; i < MAX_CHAR_NUM; i++)
	{
		if (Char[i] == '\\')j = i + 1;
		if (Char[i] == '\0')break;

	}
	//获取路径
	for (k = 0; k < j - 1; k++)
	{
		Path[k] = Char[k];
	}
	Path[k] = '\0';

	//获取名称
	for (k = 0; j < i; j++, k++)
	{
		if (Char[j] == '.') { j++; break; }
		Name[k] = Char[j];
	}
	Name[k] = '\0';

	//获取类型
	for (k = 0; j < i; j++, k++)
	{
		Type[k] = Char[j];
	}
	Type[k] = '\0';

	string file_path, file_name, file_type;
	Wchar_T_To_String(path, Path);
	Wchar_T_To_String(name, Name);
	Wchar_T_To_String(type, Type);
}
static void Int_To_Char(unsigned char dat,unsigned char *tt)
{
	unsigned char Num[2];
	unsigned char Tmp0 = dat /16,Tmp1= dat % 16;

	switch (Tmp0)
	{
		case 0:Num[0] = '0'; break;
		case 1:Num[0] = '1'; break;
		case 2:Num[0] = '2'; break;
		case 3:Num[0] = '3'; break;
		case 4:Num[0] = '4'; break;
		case 5:Num[0] = '5'; break;
		case 6:Num[0] = '6'; break;
		case 7:Num[0] = '7'; break;
		case 8:Num[0] = '8'; break;
		case 9:Num[0] = '9'; break;
		case 10:Num[0] = 'A'; break;
		case 11:Num[0] = 'B'; break;
		case 12:Num[0] = 'C'; break;
		case 13:Num[0] = 'D'; break;
		case 14:Num[0] = 'E'; break;
		case 15:Num[0] = 'F'; break;
	}

	switch (Tmp1)
	{
		case 0:Num[1] = '0'; break;
		case 1:Num[1] = '1'; break;
		case 2:Num[1] = '2'; break;
		case 3:Num[1] = '3'; break;
		case 4:Num[1] = '4'; break;
		case 5:Num[1] = '5'; break;
		case 6:Num[1] = '6'; break;
		case 7:Num[1] = '7'; break;
		case 8:Num[1] = '8'; break;
		case 9:Num[1] = '9'; break;
		case 10:Num[1] = 'A'; break;
		case 11:Num[1] = 'B'; break;
		case 12:Num[1] = 'C'; break;
		case 13:Num[1] = 'D'; break;
		case 14:Num[1] = 'E'; break;
		case 15:Num[1] = 'F'; break;
	}

	tt[0] = Num[0];
	tt[1] = Num[1];
}

static void EXP_Change(string FileName,string FilePAth)
{
	string name;
	ofstream TargetFile;
	ifstream Sourcefile;
	string line;

	int Chooce;

	Sourcefile.open(FilePAth +'\\'+ FileName, ios::in);

	//创建.c文件
	name = FileName.substr(0, FileName.size() - 4) + ".c";

	//Sourcefile.open(FileName);
	TargetFile.open(FilePAth + '\\' + name, ios::app);//在文件末尾写入

	if (TargetFile.is_open() == false) { cerr << "打开c文件失败" << endl; return;}
	if (Sourcefile.is_open() == false) { cerr << "打开EXP文件失败" << endl; return; }
	else
	{
		TargetFile << "/*" << endl;//注释文件开头
		while (getline(Sourcefile, line))
		{
			static unsigned char step = 0, addr = 0,tmp[2];

			if (addr > 0x5F)//地址超过最大值,说明复制完成
			{
				TargetFile << "};" << endl;
				break;
			}
			switch (step)
			{
				//复制注释内容
				case 0:
					//exp文件的注释由 ; 开头
					if (line[0] == ';') { TargetFile << line << endl;}
					//注释文件共84行
					if (++addr >= 84) 
					{
						step++; addr = 0;
						TargetFile << "*/" << endl;
					}
					break;
				
				//复制地址数据
				case 1:
					//获得当前数组文件名
					if (line[0] == '[')
					{
						static unsigned char Num = 0;
						
						if (addr)
						{
							TargetFile << "};" << endl;
						}
						switch (Num)
						{
						case 0:TargetFile << "const uint8_t g_cmt2300aCmtBank[CMT2300A_CMT_BANK_SIZE] = {"; break;
						case 1:TargetFile << "const uint8_t g_cmt2300aSystemBank[CMT2300A_SYSTEM_BANK_SIZE] = {"; break;
						case 2:TargetFile << "const uint8_t g_cmt2300aFrequencyBank[CMT2300A_FREQUENCY_BANK_SIZE] = {"; break;
						case 3:TargetFile << "const uint8_t g_cmt2300aDataRateBank[CMT2300A_DATA_RATE_BANK_SIZE] = {"; break;
						case 4:TargetFile << "const uint8_t g_cmt2300aBasebandBank[CMT2300A_BASEBAND_BANK_SIZE] = {"; break;
						case 5:TargetFile << "const uint8_t g_cmt2300aTxBank[CMT2300A_TX_BANK_SIZE] = {"; break;
						}
						Num++;
					}
					Int_To_Char(addr, tmp);

					if (line[0] != '0')break;
					if (line[1] != 'x')break;
					if (line[2] != tmp[0])break;
					if (line[3] != tmp[1])break;
					addr++;
					TargetFile << line.substr(6,4) << ", ";
					
					break;
					


			}
			






		}

	}
	Sourcefile.close();
	TargetFile.close();	
}

static void Find_Exp()
{

	intptr_t file_handle = 0;
	int File_Num = 0;
	struct _finddata_t file_info;//存放找到的文件信息结构体
	string p;

	string path, name, type;
	string Exp_File_Type = ".exp";
	Get_Info(path, name, type);

	if (file_handle = _findfirst(p.assign(path).append("\\*").append(Exp_File_Type).c_str(), &file_info) != -1)
	{
		do
		{
			File_Num++;
			printf("找到第 %d 个EXP文件:           ", File_Num);
			cout << file_info.name << endl;
			cout << "正在转化成.C文件..." << endl;
			EXP_Change(file_info.name, path);
			cout << endl;
			cout << "转化完成!" << endl;

		} while (_findnext(file_handle, &file_info) == 0);
		_findclose(file_handle);
		cout << endl;
		cout << endl;
		cout << endl;
		printf("共找到 %d 个EXP文件,已全部转化完成", File_Num);
	}
	else
	{
		cout << "未找到EXP文件" << endl;
	}
}



int main() 
{
	//string path, name, type;
	//string Exp_File_Type = ".exp";
	//
	////获取当前文件的文件路径, 文件名, 文件类型
	//Get_Info(path, name, type);

	Find_Exp();






	return 0;
	
}

void get_need_file(string path, vector<string>&file, string ext)
{
	    intptr_t file_handle = 0;
	    struct _finddata_t file_info;
	    string temp;
	    if ((file_handle = _findfirst(temp.assign(path).append("/*" + ext).c_str(), &file_info)) != -1)
		    {
		        do
			        {
			            file.push_back(temp.append(file_info.name));
			        } while (_findnext(file_handle, &file_info) == 0);
			        _findclose(file_handle);
			    }
	}