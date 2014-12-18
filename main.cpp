#include<cstdio>
#include<cstdlib>
#include<list>
#include<cstring>
#include<map>

struct data{
	unsigned long long address;
	int frequency;	
};
using namespace std;

int main()
{
	int frame_size[4] = {64, 128, 256, 512};
	char input[1000];
	printf("Please enter the name of trace file : ");
	scanf("%s",input);
	
	printf("%10s%10s%10s%18s\n", "size", "miss", "hit", "page fault ratio");
	int i;
	for(i=0; i<4; i++)
	{
		FILE * trace = fopen(input, "r");
		list<data> page;
		map<unsigned long long, list<data>::iterator> table;
		int hit = 0, miss = 0;
		unsigned long long address; 
		char op[20];
		while(fscanf(trace, "%s", op) != EOF)
		{	
			char addr_size[20];
			fscanf(trace, "%xl", &address);
			fscanf(trace, "%s", addr_size);
													
			data d;	
			address >>= 12;								//4KB
			if(table.count(address))					//hit
			{
				int temp = table[address]->frequency;
				temp++;
				d.frequency = temp;
				d.address = address;
				list<data>::iterator it = table[address];
				while(1)
				{
					if (it == page.end())
						break;
					else if(temp > (it->frequency))
						it++;
					else
						break;
				}
				--it;
				if(it != table[address])				//change
				{
					it++;
					page.erase(table[address]);
					page.insert(it, d);
					it--;
					table[address] = it;
				}
				else
					it->frequency = temp;
				hit++;
			}
			else										//miss
			{
				if(page.size() == frame_size[i])
				{
					table.erase(page.front().address);	//delete table 
					page.pop_front();					//pop the least used
				}				
				d.address = address;
				d.frequency = 1;				
				page.push_front(d);						//the least used
				table[address] = page.begin();	
				miss++;		
			}
		}	
		printf("%10d%10d%10d%18.10lf\n", frame_size[i], miss, hit, miss / (double) (hit + miss));		
		fclose(trace);
	}
	return 0;
}
