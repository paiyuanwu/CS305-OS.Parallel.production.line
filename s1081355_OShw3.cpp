#include <iostream>
#include <queue>
#include <cstdlib> // 亂數相關函數
#include <ctime>   //時間相關函數
#include <time.h>
#include <vector>
#include<unistd.h> 
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#pragma comment(lib, "pthreadVC2.lib")
using namespace std;
//clock_t start = time(0);

vector<bool> dbc;
struct meet
{
	int num;//number
	int time;//冷凍time
	clock_t limit;//保存期限
	int shape;//0:大豬肉 1:切塊肉 2:粽子肉
	clock_t start;
};

pthread_mutex_t lock;
int m = 15;//大豬肉數量
int n = 5;//備料區大小
meet* buf = new meet[m + 1];
queue<meet> box;//備料區
queue<meet> air;//冷凍庫
void wait(int millisecond)
{
usleep(millisecond*1000); //<unistd.h> convert to millisecond
}

int gt(clock_t start)
{
	clock_t end = (clock() * 10);
	int timeused;
	timeused = ((int)(end - start));
	return timeused;
}
//void*open(void* data)
//{
//	pthread_mutex_init(&lock, NULL);
//	pthread_mutex_lock(&lock);
//	meet* temp = (meet*)data;
//	
//	temp->shape = 0;
//	temp->limit = 0;
//	temp->time = 0;
//	temp->start = 0;
//	cout << (clock() * 10) << "ms--Pork#" << temp->num << ":waiting in the slot" << endl;
//	
//
//	pthread_mutex_unlock(&lock);
//	return NULL;
//}

void* doSomeThing(void* data)
{
	meet* temp = (meet*)data;
	pthread_mutex_init(&lock,NULL);
	//while (true)
	//{
		//if (temp->shape == 1)
		//	break;
		pthread_mutex_lock(&lock);
		/*for (int i = 1; i <= box.size(); i++)
		{
			meet temp = box.front();
			if (temp.shape == 0)
			{
				cout << (clock() * 10) << "ms--Pork#" << temp.num << ":enters the CUTTER" << endl;
				temp.shape = 1;


				int w = (rand() % 16 + 15) * 10;
				cout << (clock() * 10) << "ms--CUTTER: cutting... cutting..." << "Pork#" << temp.num << "--" << w << endl;
				wait(w);
				cout << (clock() * 10) << "ms--" << "Pork#"<<temp.num<<": leaves CUTTER (complete 1st stage)"<< endl;

				box.push(temp);
				cout << (clock() * 10) << "ms --Pork#" << temp.num << ":waiting in the slot(cutted)" << endl;
				box.pop();
			}
			else
			{
				box.push(temp);
				box.pop();
			}
		}*/
		//meet* temp = (meet*)data;
		//temp->shape = 1;
		cout << (clock() * 10) << "ms--Pork#" << temp->num << ":enters the CUTTER" << endl;
		int w = (rand() % 16 + 15) * 10;
		cout << (clock() * 10) << "ms--CUTTER: cutting... cutting..." << "Pork#" << temp->num << "--" << w*10 << endl;
		wait(w);
		cout << (clock() * 10) << "ms--" << "Pork#" << temp->num << ": leaves CUTTER (complete 1st stage)" << endl;
		return data;
		pthread_exit(0);
		pthread_mutex_unlock(&lock);
		
		//return data;
	//}
	//return NULL;
}

void* finish(void* data)
{
	pthread_mutex_init(&lock, NULL);
	pthread_mutex_lock(&lock);
	/*for (int i = 1; i <= box.size(); i++)
	{
		meet temp = box.front();
		if (temp.shape == 1)
		{

			box.push(temp);
			cout << (clock() * 10) << "ms--Pork#" << temp.num << ":enters to the factory (PACKER)" << endl;
			temp.shape = 2;
			pthread_create(&fin, NULL, finish, (void*)&temp);
			
			int w = (rand() % 6 + 5) * 100;
			wait(w);
			cout << (clock()*10) << "ms--Pork#" << temp.num << ": leaves PACKER(Complete)" << endl;
			
			box.push(temp);
			box.pop();
		}
		else
		{
			box.push(temp);
			box.pop();
		}
	}*/

	meet* temp = (meet*)data;
	//temp->shape = 2;
	cout << (clock() * 10) << "ms--Pork#" << temp->num << ":enters to the factory (PACKER)" << endl;
	temp->shape = 2;
	int w = (rand() % 6 + 5) * 100;
	wait(w);
	cout << (clock()*10) << "ms--Pork#" << temp->num << ": leaves PACKER(Complete)" << endl;
	pthread_exit(data);
	//pthread_exit(temp);
	pthread_mutex_unlock(&lock);
	return data;
	//return NULL;
}

int main(int argc, char* argv[])
{
	m =atoi(argv[1]);
	n =atoi(argv[2]);
	srand(10);
	//int m = 20;//大豬肉數量
	//int n = 5;//備料區大小
	//meet* buf = new meet[m + 1];
	//queue<meet> box;//備料區
	//queue<meet> air;//冷凍庫
	pthread_t op;
	pthread_t tid;
	pthread_t fin;
	cout << (clock() * 10) << "ms--CUTTER: under maintenance." << endl;
	cout << (clock() * 10) << "ms-- PACKER: under maintenance." << endl;
	cout << (clock() * 10) << "ms-- PACKER: under reviewing together..." << endl;
	cout << (clock() * 10) << "ms-- CUTTER: under reviewing together..." << endl;
	//pthread_create(&op, NULL, doSomeThing, (void*)&temp);
	for (int i = 1; i <= m; i++) //大豬肉倉儲分類作業
	{
		wait((rand() % 6 + 5) * 10);
		if (box.size() < n)
		{
			meet temp;
			temp.num = i;
			//pthread_create(&op, NULL, open, (void*)&temp);
			//box.push(temp);
			//meet temp;
			//temp.num = i;
			temp.shape = 0;
			temp.limit = 0;
			temp.time = 0;
			temp.start = 0;
			cout << (clock() * 10) << "ms--Pork#" << temp.num << ":waiting in the slot" << endl;
			box.push(temp);
		}
		else
		{
			meet temp;
			temp.num = i;
			temp.shape = 0;
			temp.limit = (clock() * 10);
			temp.time = (rand() % 21 + 30) * 10;
			temp.start = (clock() * 10);
			air.push(temp);
		}
	}
	//box.pop();
	/*for (int i = 1; i <= air.size(); i++)
	{
		pthread_attr_t ac;
		pthread_attr_init(&ac);
		pthread_create(&ac,NULL, doSomeThing, NULL);
	}*/
	//pthread_create(&robotThread, NULL, robot, NULL);

	//for (int i = 1; i <= air.size(); i++)
	//{
	//	/* pthread_attr_t ac;
	//	 pthread_attr_init(&ac);
	//	 pthread_create(&tid[i], &ac, doSomeThing, NULL);*/
	//	meet temp = air.front();
	//	if (gt(temp.limit) >= 60000)
	//		air.pop();
	//	else if (gt(temp.start) >= temp.time)
	//	{
	//		if (box.size() < n)
	//		{
	//			/*pthread_create(&tid, NULL, doSomeThing, (void*)&temp);*/
	//			temp.limit = 0;
	//			temp.time = 0;
	//			temp.start = 0;
	//			box.push(temp);
	//			cout << (clock() * 10) << "ms--Pork#" << temp.num << ":waiting in the slot" << endl;
	//			air.pop();
	//		}
	//		else
	//		{
	//			temp.start = (clock() * 10);
	//			air.push(temp);
	//			air.pop();
	//		}
	//	}
	//	else
	//	{
	//		//temp.start = clock();
	//		air.push(temp);
	//		air.pop();
	//	}
	//}
	/*meet temp = box.front();
	pthread_create(&tid, NULL, doSomeThing, (void*)&temp);*/
	//for (int i = 1; i <= box.size(); i++)
	while(true)
	{
		for (int i = 1; i <= air.size(); i++)
		{
			meet temp = air.front();
			if (gt(temp.limit) >= 6000000)
				air.pop();
			else if (gt(temp.start) >= temp.time)
			{
				if (box.size() < n)
				{
					/*pthread_create(&tid, NULL, doSomeThing, (void*)&temp);*/
					temp.limit = 0;
					temp.time = 0;
					temp.start = 0;
					box.push(temp);
					cout << (clock() * 10) << "ms--Pork#" << temp.num << ":waiting in the slot" << endl;
					air.pop();
				}
				else
				{
					temp.start = (clock() * 10);
					air.push(temp);
					air.pop();
				}
			}
			else
			{
				//temp.start = clock();
				air.push(temp);
				air.pop();
			}
		}
		if (dbc.size() == m)
			break;

		meet temp = box.front();
		if (temp.shape == 0)
		{
			//void* retval;
			int ret;
			int before = temp.num;
			//pthread_join(tid, (void**)&threadResult);
			//ret = pthread_join(tid, (void**)&temp);
			/*if (ret == 0)
				printf("The thread was canceled - ");*/
			/*cout << (clock() * 10) << "ms--Pork#" << temp.num << ":enters the CUTTER" << endl;
			temp.shape = 1;*/
			/*cout << (clock() * 10) << "ms--Pork#" << temp.num << ":enters the CUTTER" << endl;
			temp.shape = 1;*/
			pthread_create(&tid, NULL, doSomeThing, (void*)&temp);
			ret = pthread_join(tid, (void**)&temp);
			//int pthread_join(pthread_t tid, NULL);
			//tmp1 = pthread_join(tid, &retval);
			if (ret == 0)
			{
				temp.shape = 1;
				temp.num = before;
			}
			//temp.shape = 1;
			//
			//int w = (rand() % 16 + 15) * 10;
			//cout << clock() << "ms--CUTTER: cutting... cutting..." << "Pork#" << temp.num << "--" << w << endl;
			//wait(w);
			//cout << clock()<< "ms--" << "Pork#"<<temp.num<<": leaves CUTTER (complete 1st stage)"<< endl;
			//
			box.push(temp);
			cout << (clock() * 10+10) << "ms --Pork#" << temp.num << ":waiting in the slot(cutted)" << endl;

			box.pop();
		}
		else if (temp.shape == 1)
		{
		//	cout << (clock() * 10) << "ms--Pork#" << temp.num << ":enters to the factory (PACKER)" << endl;
			int ret;
			int before = temp.num;
			pthread_create(&fin, NULL, finish, (void*)&temp);
			ret = pthread_join(fin, (void**)&temp);
			//
			/*int w = (rand() % 6 + 5) * 100;
			wait(w);
			cout << clock() << "ms--Pork#" << temp.num << ": leaves PACKER(Complete)" << endl;*/
			//
			
			if (ret == 0 )
			{
				temp.shape = 2;
				temp.num = before;
				box.pop();
				dbc.push_back(1);
			}
			else
			{
				box.push(temp);
				box.pop();
			}
			
		}
		

	}


	//meet temp = box.front();
	//pthread_create(&fin, NULL, doSomeThing, (void*)&temp);

	//for (int i = 1; i <= box.size(); i++)
	//{
	//	meet temp = box.front();
	//	if (temp.shape == 1)
	//	{

	//		//box.push(temp);
	//		cout << (clock() * 10) << "ms--Pork#" << temp.num << ":enters to the factory (PACKER)" << endl;
	//		temp.shape = 2;
	//		pthread_create(&fin, NULL,finish, (void*)&temp);
	//		//
	//		/*int w = (rand() % 6 + 5) * 100;
	//		wait(w);
	//		cout << clock() << "ms--Pork#" << temp.num << ": leaves PACKER(Complete)" << endl;*/
	//		//
	//		box.push(temp);
	//		box.pop();
	//	}
	//	else
	//	{
	//		box.push(temp);
	//		box.pop();
	//	}
	//}
}


