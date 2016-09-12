#include<stdio.h>
#include<stdlib.h>
#define max 100 //max 100 nodes
#define maxip 128
struct station
{
    int ID,ip,ip_start,ip_end;
    int available,level,buddy,valid;
    int map[max][3];    
}node[max];

int globmap[max][3];
int n=-1;

void disp()
{
    int i,i1,j;
    printf("\nID   S   E   B");
    for(i=0;i<=n;i++)
    {   
        if(node[i].valid)
        {
            printf("\n");
            /*for(i1=0;i1<=n;i1++)
            {
                for(j=0;j<3;j++)
                    printf("%d  ",node[i].map[i1][j]);
                printf("\n");
            }*/
            printf("%d  %d  %d  %d",node[i].ID,node[i].ip_start,node[i].ip_end,node[i].buddy);
        }
    }
}

void update_map(int x)
{
    int i,j,k;
    for(k=0;k<=n;k++)
        for(i=0;i<=n;i++)
            for(j=0;j<3;j++)
                node[k].map[i][j]=globmap[i][j];
}

void init()
{
    int i,j;
    node[n].ID=n;
    node[n].valid=1;
    node[n].level=n%5;
    node[n].available=1;
    if(n==0)
    {
        node[n].ip_start=0;
        node[n].ip_end=maxip;
        node[n].ip=0;
        node[n].buddy=-1;
        node[n].map[0][0]=node[n].ID;
        node[n].map[0][1]=node[n].ip_start;
        node[n].map[0][2]=node[n].ip_end;
        globmap[0][0]=node[n].ID;
        globmap[0][1]=node[n].ip_start;
        globmap[0][2]=node[n].ip_end;
    }
    else
    {
        node[n].buddy=(0+rand()%n);//-1;
        int b,x,ch=0,i=0;
        while(ch==0 && i<n)
        {
            if(node[node[n].buddy].available==1 && node[node[n].buddy].valid==1)
            {
                ch=1;
                b=node[n].buddy;        //assign buddy
                x=(node[b].ip_end-node[b].ip_start)/2;    //binary split ip adress block
                node[n].ip_start=node[b].ip_start+x+1;
                node[n].ip_end=node[b].ip_end;
                node[n].ip=node[n].ip_start;
                node[b].ip_end=node[b].ip_start+x;
                node[b].map[b][0]=node[b].ID;             //update buddy's address map table
                node[b].map[b][1]=node[b].ip_start;
                node[b].map[b][2]=node[b].ip_end;
                node[b].map[n][0]=node[n].ID;
                node[b].map[n][1]=node[n].ip_start;
                node[b].map[n][2]=node[n].ip_end;
                globmap[b][0]=node[b].ID;             //update global address map table
                globmap[b][1]=node[b].ip_start;
                globmap[b][2]=node[b].ip_end;
                globmap[n][0]=node[n].ID;
                globmap[n][1]=node[n].ip_start;
                globmap[n][2]=node[n].ip_end;
                if(node[b].ip_start==node[b].ip_end)
                    node[b].available=0;
                if(node[n].ip_start==node[n].ip_end)
                    node[n].available=0;
                for(i=0;i<=n;i++)
                    for(j=0;j<3;j++)
                        node[n].map[i][j]=node[b].map[i][j];
            }
            else
            {
                node[n].buddy++; 
                node[n].buddy=node[n].buddy%n;
                i++;
            }   
        }
        if(i==n)
        {
            printf("\nNETWORK IS FULL");
            n--;
        }
    }
}

void add()
{
    n++;
    init(); 
    disp();
}

void rmv(int id)
{
    int i,j,k,flag=0;
    if(node[id].valid==0)
        printf("\nNode doesn't exist");
    else
    {
        node[id].valid=0;
        for(i=0;i<=n;i++)
        {
            if(node[i].valid==1)
            {
                //printf("HERE");
                if(i==id)
                    continue;
                if(node[id].ip_start-1==node[i].ip_end)     //add back ip block to buddy/other node
                {
                    node[i].ip_end=node[id].ip_end;
                    flag=1;
                }
                else if(node[id].ip_end+1==node[i].ip_start)
                {
                    node[i].ip_start=node[id].ip_start;
                    flag=1;
                }
                if(flag)
                {
                    globmap[i][1]=node[i].ip_start;
                    globmap[i][2]=node[i].ip_end;
                    //printf("here");
                    update_map(i);
                    printf("\nNODE REMOVED"); 
                    break;
                }   
            }       
        }
    }
}

int main()
{
    int yn;
    int ch,id;
    do{
        printf("\nENTER CHOICE- 1.Add a node  2.Remove a node  ");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:add();
                    break;
            case 2:printf("\nEnter node ID");
                    scanf("%d",&id);
                    rmv(id);
                    break;
            default: break; 
        }
        printf("\n\nCONTINUE?(0/1) ");
        scanf("%d",&yn);
    }while(yn==1);
}
