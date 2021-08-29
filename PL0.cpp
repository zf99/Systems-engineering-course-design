//A.2     C      ��   ��

/*��������л���:
*1Visual C++6.0,VisualC++.NET and Visual C++.NET 2003
*WinNT, Win 200, WinXP and  Win2003 
*2 gcc version 3.3.2  20031022(Red Hat Linux 3.3.2-1)
*Redhat Fedora core 1
*Intel 32 platform
*ʹ�÷���:
*���к�����PL/0 Դ�����ļ���
*�ش��Ƿ�������������
*�ش��Ƿ�������ֱ�
*fa.tmp ������������
*fa1.tmp  ���Դ�ļ�������ж�Ӧ���׵�ַ
*fa2.tmp  ������ 
*fas.tmp  ������ֱ�
*/
#include<stdio.h>
#include"pl0.h"
#include"string.h"
/*����ִ��ʱʹ�õ�ջ*/
#define stacksize 500
int main()
{
	bool nxtlev[symnum];
	printf("Input pl/0 file ?");
	scanf("%s",fname);                                     /*�����ļ���*/
	fin=fopen(fname,"r");
	if(fin)
	{
		printf("List object code ?(Y/N)");                /*�Ƿ�������������*/
		scanf("%s",fname);
		listswitch=(fname[0]=='y'||fname[0]=='Y');
		
		printf("List symbol table ? (Y/N)");             /*�Ƿ�������ֱ�*/
		scanf("%s",fname);
		tableswitch=(fname[0]=='y'||fname[0]=='Y');
		 
		fa1=fopen("fa1.tmp","w");
		fprintf(fa1,"Iput pl/0 file ?");
		fprintf(fa1,"%s\n", fname);
		init();                                          /*��ʼ��*/
		err=0;
		cc=cx=ll=0;
		ch=' ';
		if(-1!=getsym())
		{
			fa=fopen("fa.tmp","w");
			fas=fopen("fas.tmp","w");
			
			res=fopen("res.s","w");//-----------------
			fprintf(res,".data\n");//-----------------
			
			addset(nxtlev,declbegsys,statbegsys,symnum);
			nxtlev[period]=true;    
			if(-1==block(0,0,nxtlev))			/*���ñ������*/
			{
				fclose(fa);
				fclose(fa1);
				fclose(fas);
				fclose(fin);
				printf("\n");
				return 0;
			}
			fclose(fa);
			fclose(fa1);
			fclose(fas);
			if(sym!=period)
			{
				error(9);
			}
			if(err==0)
			{
				fa2=fopen("fa2.tmp", "w");
				interpret();
				fclose(fa2);
			}
			else
			{
				printf("Errors in pl/0 program");
			}
		}
		fclose(res);
		fclose(fin);
	}
	else
	{
		printf("Can't open file! \n");
	}
	printf("\n");
	return 0;
}
/*
*��ʼ��
*/
void init()
{
	int i;
	for(i=0;i<=255;i++)
	{
		ssym[i]=nul;
	}
	ssym['+']=plus;
	ssym['-']=minus;
	ssym['*']=times;
	ssym['/']=slash;
	ssym['(']=lparen;
	ssym[')']=rparen;
	ssym['=']=eql;
	ssym[',']=comma;
	ssym['.']=period;
	ssym['#']=neq;
	ssym[';']=semicolon;
	/*���ñ���������,������ĸ˳��,�����۰����*/
	strcpy(&(word[0][0]),"begin");
	strcpy(&(word[1][0]),"call");
	strcpy(&(word[2][0]),"const");
	strcpy(&(word[3][0]),"do");
	strcpy(&(word[4][0]),"end");
	strcpy(&(word[5][0]),"if");
	strcpy(&(word[6][0]),"odd");
	strcpy(&(word[7][0]),"procedure");
	strcpy(&(word[8][0]),"read");
	strcpy(&(word[9][0]),"then");
	strcpy(&(word[10][0]),"var");
	strcpy(&(word[11][0]),"while");
	strcpy(&(word[12][0]),"write");
	/*���ñ����ַ���*/
	wsym[0]=beginsym;
	wsym[1]=callsym;
	wsym[2]=constsym;
	wsym[3]=dosym;
	wsym[4]=endsym;
	wsym[5]=ifsym;
	wsym[6]=oddsym;
	wsym[7]=procsym;
	wsym[8]=readsym;
	wsym[9]=thensym;
	wsym[10]=varsym;
	wsym[11]=whilesym;
	wsym[12]=writesym;

	/*����ָ������*/
	strcpy(&(mnemonic[lit][0]),"lit");
	strcpy(&(mnemonic[opr][0]),"opr");
	strcpy(&(mnemonic[lod][0]),"lod");
	strcpy(&(mnemonic[sto][0]),"sto");
	strcpy(&(mnemonic[cal][0]),"cal");
	strcpy(&(mnemonic[inte][0]),"int");
	strcpy(&(mnemonic[jmp][0]),"jmp");
	strcpy(&(mnemonic[jpc][0]),"jpc");

	/*���÷��ż�*/
	for(i=0;i<symnum;i++)
	{
		declbegsys[i]=false;
		statbegsys[i]=false;
		facbegsys[i]=false;
	}

	/*����������ʼ���ż�*/
	declbegsys[constsym]=true;
	declbegsys[varsym]=true;
	declbegsys[procsym]=true;
	/*������俪ʼ���ż�*/
	statbegsys[beginsym]=true;
	statbegsys[callsym]=true;
	statbegsys[ifsym]=true;
	statbegsys[whilesym]=true;
	/*�������ӿ�ʼ���ż�*/
	facbegsys[ident]=true;
	facbegsys[number]=true;
	facbegsys[lparen]=true;
}
 /*
  *������ʵ�ּ��ϵļ�������
  */
int inset(int e,bool* s)
{
    return s[e];
}
int addset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]||s2[i];  
    }
    return 0;
}
int subset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&(!s2[i]);
    }
    return 0; 
}
int mulset(bool* sr,bool* s1,bool* s2,int n)
{
    int i;
    for(i=0;i<n;i++)
    {
       sr[i]=s1[i]&&s2[i];  
    } 
    return 0;
}
/*
 *������������ӡ����λ�úʹ������
 */
void error(int n)
 
{
	char space[81];
	memset(space,32,81); printf("-------%c\n",ch);
	space[cc-1]=0;//����ʱ��ǰ�����Ѿ����꣬����cc-1
	printf("****%s!%d\n",space,n);
	err++;
}
/*
 *  ©���ո񣬶�ȡһ���ַ�
 *
 *  ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
 *
 *  ������getsym����
 */
int getch()
{
	if(cc==ll)
    {
		if(feof(fin))
		{
			printf("program incomplete");
			return -1;
		}
		ll=0;
		cc=0;
		printf("%d ",cx );
		fprintf(fa1,"%d ",cx);
		ch=' ';
		while(ch!=10)
		{
			//fscanf(fin,"%c",&ch)
            if(EOF==fscanf(fin,"%c",&ch))
			{
				line[ll]=0;
				break;
			}
			printf("%c",ch);
			fprintf(fa1,"%c",ch);
			line[ll]=ch;
			ll++;
		}
		printf("\n");
		fprintf(fa1,"\n");
	}
	ch=line[cc];
	cc++;
	return 0;
}
 /*�ʷ���������ȡһ������
 */

int getsym()
{
	int i,j,k;
	while( ch==' '||ch==10||ch==9)
	{
		getchdo;
	}
	if(ch>='a'&&ch<='z')
	{ 
		k=0;
		do{
			if(k<al)
			{ 
				a[k]=ch;
				k++;
			}
			getchdo; 
		}while(ch>='a'&&ch<='z'||ch>='0'&&ch<='9');
		a[k]=0;
		strcpy(id,a);
		i=0;
		j=norw-1;
		do{
			k=(i+j)/2;
			if(strcmp(id,word[k])<=0)
			{
				j=k-1;
			}
			if(strcmp(id,word[k])>=0)
			{
				i=k+1;
			}

		}while(i<=j);
		if(i-1>j)
		{
			sym=wsym[k];
		}
		else
		{
			sym=ident;
		}
	}
	else
	{
		if(ch>='0'&&ch<='9')
		{
			k=0;
			num=0;
			sym=number;
			do{
				num=10*num+ch-'0';
				k++;
				getchdo;
			}while(ch>='0'&&ch<='9'); /*��ȡ���ֵ�ֵ*/
			k--;
			if(k>nmax)
			{
				error(30);
			}
		}
		else
		{
			if(ch==':')             /*��⸳ֵ����*/
			{
				getchdo;
				if(ch=='=')
				{
					sym=becomes;
					getchdo;
				}
				else
				{
					sym=nul;            /*����ʶ��ķ���*/
				}
			}
			else
			{
				if(ch=='<')         /*���С�ڻ�С�ڵ��ڷ���*/
				{
					getchdo;
					if(ch=='=')
					{
						sym=leq;
						getchdo;
					}
					else
					{ 
						sym=lss;
					}
				}
				else
				{
					if(ch=='>')          /*�����ڻ���ڵ��ڷ���*/
					{
						getchdo;
						if(ch=='=')
						{
							sym=geq;
							getchdo;
						}
						else
						{
						    sym=gtr;
						}
					}
					else
					{
						sym=ssym[ch];/* �����Ų�������������ʱ��ȫ�����յ��ַ��Ŵ���*/
						//getchdo;
						//richard
						if(sym!=period)
						{
							getchdo;
						}
						//end richard
					}
				}
			}
		}
	}
	return 0;
}
/*
*�������������
*
*x:instruction.f;
*y:instruction.l;
*z:instruction.a;
*/
int gen(enum fct x,int y,int z)
{
	if(cx>=cxmax)
	{
		printf("Program too long"); /*�������*/
		return -1;
	}
	code[cx].f=x;
	code[cx].l=y;
	code[cx].a=z;
	code[cx].flag=-1;//��ʼ��������������Ϊ-1 ��ͬʱҲ��ָ��δ�����룬
	cx++;
	return 0;
}
/*
*���Ե�ǰ�����Ƿ�Ϸ�
*
*��ĳһ���֣���һ����䣬һ������ʽ����Ҫ����ʱʱ����ϣ����һ����������ĳ����
*���ò��ֵĺ�����ţ� test ���������⣬���Ҹ��𵱼�ⲻͨ��ʱ�Ĳ��ȴ�ʩ
*��������Ҫ���ʱָ����ǰ��Ҫ�ķ��ż��ϺͲ����õļ��ϣ���֮ǰδ��ɲ��ֵĺ��
*���ţ����Լ���ͨ��ʱ�Ĵ����
*
*S1��������Ҫ�ķ���
*s2:�������������Ҫ�ģ�����Ҫһ�������õļ���
*n:�����
*/


int test(bool* s1,bool* s2,int n)
{
    if(! inset(sym,s1))
    {
		error(n);
		/*����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ���*/
		while((! inset(sym,s1))&&(! inset(sym,s2)))
		{
			getsymdo; 
		}
    }
    return 0;
}
/*
 *�����������
 *
 *lev:��ǰ�ֳ������ڲ�
 *tx:���ֱ���ǰβָ��
 *fsys:��ǰģ�������ż���
 */
int block(int lev,int tx,bool* fsys)
{
    int i;
    int dx;                         /*���ַ��䵽����Ե�ַ*/
    int tx0;                        /*������ʼtx*/
    int cx0;                        /*������ʼcx*/
    bool nxtlev[symnum];            /*���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������
                                      ʵ�֣����ݽ�������ָ�룬Ϊ��ֹ�¼������ı��ϼ�������
                                      ���ϣ������µĿռ䴫�ݸ��¼�����*/
    dx=3;
    tx0=tx;                         /*��¼�������ֵĳ�ʼλ��*/
    table[tx].adr=cx;
    gendo(jmp,0,0);
    if(lev > levmax)
    {
		error(32);                                                                                                                                         
    }
    do{
        if(sym==constsym)         /*�յ������������ţ���ʼ������������*/
        {
			getsymdo;
			do{
                constdeclarationdo(&tx,lev,&dx);   /*dx��ֵ�ᱻconstdeclaration�ı䣬ʹ��
                                                     ָ��*/
                while(sym==comma)
                {
                   getsymdo;
                   constdeclarationdo(&tx,lev,&dx);
                }
                if(sym==semicolon)
                {
                  getsymdo;
				}
				else
				{
					error(5); /*©���˶��Ż��߷ֺ�*/
				}
			}while(sym==ident);
		}
		if(sym==varsym)/*�յ������������ţ���ʼ������������*/
		{
			getsymdo;
			do{
				vardeclarationdo(&tx,lev,&dx);
				while(sym==comma)
				{
					getsymdo;
					vardeclarationdo(&tx,lev,&dx);
				}
				if(sym==semicolon)
				{
					getsymdo;
				}
				else
				{
					error(5);
				}
			}while(sym==ident);
		}
		while(sym==procsym)/*�յ������������ţ���ʼ������������*/
		{
			getsymdo;
			if(sym==ident)
			{
				enter(procedur,&tx,lev,&dx);/*��¼��������*/
				getsymdo;
			}
			else
			{
				error(4);/*procedure��ӦΪ��ʶ��*/
			}
			if(sym==semicolon)
			{
				getsymdo;
			}
			else
			{
				error(5);/*©���˷ֺ�*/
			}
			memcpy(nxtlev,fsys,sizeof(bool)*symnum);
			nxtlev[semicolon]=true;
			if(-1==block(lev+1,tx,nxtlev))
			{
				return -1;/*�ݹ����*/
			}
            if(sym==semicolon)
            {
                getsymdo;
                memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
                nxtlev[ident]=true;
                nxtlev[procsym]=true;
                testdo(nxtlev,fsys,6);
             }
             else
             {
                 error(5);                       /*©���˷ֺ�*/
             }
        }
        memcpy(nxtlev,statbegsys,sizeof(bool)*symnum);
        nxtlev[ident]=true;
        nxtlev[period]=true;
        testdo(nxtlev,declbegsys,7);
    }while(inset(sym,declbegsys));                /*ֱ��û����������*/
    code[table[tx0].adr].a=cx;                    /*��ʼ���ɵ�ǰ���̴���*/
    table[tx0].adr=cx;                            /*��ǰ���̴����ַ*/
    table[tx0].size=dx;                           /*����������ÿ����һ�����������dx����1,���������Ѿ�����,dx���ǵ�ǰ�������ݵ�size*/
        
    cx0=cx;
    gendo(inte,0,dx);                             /*���ɷ����ڴ����*/
    if(tableswitch)                               /*������ֱ�*/
    {
        printf("TABLE:\n");
        if(tx0+1>tx)
        {
			printf("NULL\n");
        }
        for(i=tx0+1;i<=tx;i++)
        {
            switch(table[i].kind)
            {
                case constant:
					printf("%d const %s  ",i,table[i].name);
					printf("val=%d\n",table[i].val);
					fprintf(fas,"%d const %s",i,table[i].name);
					fprintf(fas,"val=%d\n",table[i].val);
                    break;
                case variable:
                    printf("%d var  %s  ",i,table[i].name);
                    printf("lev=%d addr=%d\n",table[i].level,table[i].adr);
                    fprintf(fas,"%d var %s",i,table[i].name);
                    fprintf(fas,"lev=%d addr=%d\n",table[i].level,table[i].adr);
                    break;
                case procedur:
                    printf("%d proc%s",i,table[i].name);
                    printf("lev=%d addr=%d size=%d\n",table[i].level,table[i].adr,table[i].size);
					fprintf(fas,"%d proc%s",i,table[i].name);
					fprintf(fas,"lev=%d adr=%d size=%d \n",table[i].level,table[i].adr,table[i].size);
					break;
			}
		}
		printf("\n");
	}
	/*���������Ϊ�ֺŻ�end*/
	memcpy(nxtlev,fsys,sizeof(bool)*symnum);/*ÿ��������ż��Ͷ������ϲ������ż��ͣ��Ա㲹��*/
	nxtlev[semicolon]=true;
	nxtlev[endsym]=true;
	statementdo(nxtlev,&tx,lev);
	gendo(opr,0,0); /*ÿ�����̳��ڶ�Ҫʹ�õ��ͷ����ݶ�����*/
	memset(nxtlev,0,sizeof(bool)*symnum); /*�ֳ���û�в��ȼ���*/
	test(fsys,nxtlev,8);                  /*�����������ȷ��*/
	listcode(cx0);                        /*�������*/
	return 0;
}
/*
*�����ֱ��м���һ��
*
*k:��������const,var or procedure
*ptx:���ֱ�βָ���ָ�룬Ϊ�˿��Ըı����ֱ�βָ�����ֵ
*lev:�������ڵĲ�Σ��Ժ����е�lev��������
*pdx:Ϊ��ǰӦ����ı�������Ե�ַ�������Ҫ����1
*/
void enter (enum object k,int *ptx,int lev, int *pdx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name,id);       /*ȫ�ֱ���id���Ѵ��е�ǰ���ֵ�����*/
	table[(*ptx)].kind=k;
	switch(k)
	{
		case constant:                      /*��������*/
			if (num>amax)
			{
				error(31);
				num=0;
			}
			table[(*ptx)].val=num;
			break;
		case variable:                     /*��������*/
			table[(*ptx)].level=lev;
			table[(*ptx)].adr=(*pdx);
			(*pdx)++;
			break;                          /*��������*/
		case procedur:
			table[(*ptx)].level=lev;
			break;
	}

}
/*
 *�������ֵ�λ��
 *�ҵ��򷵻������ֱ��е�λ�ã����򷵻�0
 *
 *idt: Ҫ���ҵ�����
 *tx:����ǰ���ֱ�βָ��
 */
int position(char *  idt,int  tx)
{
	int i;
	strcpy(table[0].name,idt);
	i=tx;
	while(strcmp(table[i].name,idt)!=0)
	{
		i--;
	}
	return i;
}
/*
 *������������
 */
int constdeclaration(int *  ptx,int lev,int *  pdx)
{
	if(sym==ident)
	{
		getsymdo;
		if(sym==eql ||sym==becomes)
		{
			if(sym==becomes)
			{
				error(1);                     /*��=д�����ˣ�=*/
			}
			getsymdo;
			if(sym==number)
			{
				enter(constant,ptx,lev,pdx);
				//-----------------------------------
				fprintf(res,"%s:\t.word %d\n",id,num);	//resָ��������� 
				//----------------------------------- 
				getsymdo;
			}
			else
			{
				error(2);                  /*����˵��=��Ӧ������*/
			}         
		}
		else
		{
			error(3);                       /*����˵����ʶ��Ӧ��=*/
		}
	}
	else
	{
		error(4);                        /*const��Ӧ�Ǳ�ʶ*/
	}
	return 0;
}
/*
 *
 */
int vardeclaration(int * ptx,int lev,int * pdx)
{
	if(sym==ident)
 	{
 		enter(variable,ptx,lev,pdx);//��д���ֱ�
 		getsymdo;
 	}
 	else
 	{
 		error(4);
    }
    return 0;
}
 /*
  *����Ŀ������嵥
  */
void listcode(int cx0)
{
	int i;
   	if (listswitch)
   	{
   		for(i=cx0;i<cx;i++)
   	 	{
   	 		printf("%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
   	 		fprintf(fa,"%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
   	    }
   	 }
}
/*
*��䴦��
*/
int statement(bool* fsys,int * ptx,int lev)
{
	int i,cx1,cx2;
   	bool nxtlev[symnum];
   	if(sym==ident)
   	{
		i=position(id,*ptx);
   	 	if(i==0)
   	 	{
   	 		error(11);
   	 	}
   	 	else
   	 	{
   	 		if(table[i].kind!=variable)
   	 		{
   	 		 	error(12);
   	 		 	i=0;
   	 		}
   	 		else
   	 		{
   	 			getsymdo;
   	 			if(sym==becomes)
   	 			{
   	 				getsymdo;
   	 			}
   	 			else
   	 			{
   	 			 	error(13);
   	 			}
   	 			memcpy(nxtlev,fsys,sizeof(bool)* symnum);
   	 			expressiondo(nxtlev,ptx,lev);
   	 			if(i!=0)
   	 			{
   	 			 	gendo(sto,lev-table[i].level,table[i].adr);
   	 			}
   	 	    }
   		}
    }
    else
    {
    	if(sym==readsym)
    	{
    	 	getsymdo;
    	 	if(sym!=lparen)
    	 	{
    	 	   error(34);
			}
    	    else
			{
    	   		do{
    	   			getsymdo;
    	   	   		if(sym==ident)
    	   	   		{
    	   	   	  		i=position(id, *ptx);
    	   	   		}
    	   	   		else
    	   	   		{
    	   	   	  		i=0;
    	   	   		}
    	   	   		if(i==0)
    	   	   		{
    	   	   	 		error(35);
    	   	   		}
    	   	   		else
    	   	   		{
    	   	   			gendo(opr,0,16);
						gendo(sto,lev-table[i].level,table[i].adr);	/* ���浽����*/
					}
					getsymdo;
				}while (sym==comma);	/*һ��read���ɶ�������� */
			}
			if(sym!=rparen)
			{
				error(33);			/* ��ʽ����Ӧ��������*/
				while(!inset(sym,fsys))/* �������ȣ�ֱ���յ��ϲ㺯���ĺ������*/
				{
					getsymdo;
				}
			}
			else
			{
				getsymdo;
			}
		}
		else
		{
			if(sym==writesym)			/* ׼������write��䴦������read����*/
			{
				getsymdo;
				if(sym==lparen)
				{
					do{
						getsymdo;
						memcpy(nxtlev,fsys,sizeof(bool)*symnum);
						nxtlev[rparen]=true;
						nxtlev[comma]=true;		/* write�ĺ������Ϊ��or��*/
						expressiondo(nxtlev,ptx,lev);/* ���ñ���ʽ�������˴���read��ͬ��readΪ��������ֵ*/
						gendo(opr,0,14);/* �������ָ����ջ����ֵ*/
					}while(sym==comma);
					if(sym!=rparen)
					{
						error(33);/* write()ӦΪ��������ʽ*/
					}
					else
					{
						getsymdo;
					}
				}
				gendo(opr,0,15);		/* �������*/
			}
			else
			{
				if(sym==callsym)		/* ׼������call��䴦��*/
				{
					getsymdo;
					if(sym!=ident)
					{
						error(14);           /*call��ӦΪ��ʶ��*/
					}
					else
					{
						i=position(id,*ptx);
						if(i==0)
						{
							error(11);          /*����δ�ҵ�*/
						}
						else
						{
							if(table[i].kind==procedur)
							{
								gendo(cal,lev-table[i].level,table[i].adr);  /*����callָ��*/
							}
							else
							{
								error(15);      /*call���ʶ��ӦΪ����*/
							}
						}
						getsymdo;
					}
				}
				else
				{
					if(sym==ifsym)     /*׼������if��䴦��*/
					{
						getsymdo;
						memcpy(nxtlev,fsys,sizeof(bool)*symnum);
						nxtlev[thensym]=true;
						nxtlev[dosym]=true;    /*�������Ϊthen��do*/
						conditiondo(nxtlev,ptx,lev);   /*���������������߼����㣩����*/
						if(sym==thensym)
						{
							getsymdo;
						}
						else
						{
							error(16);          /*ȱ��then*/
						}
						cx1=cx;                /*���浱ǰָ���ַ*/
						gendo(jpc,0,0);        /*����������תָ���ת��ַ��д0*/
						statementdo(fsys,ptx,lev);   /*����then������*/
						code[cx1].a=cx;      /*��statement������cxΪthen�����ִ��
                            ���λ�ã�������ǰ��δ������ת��ַ*/
					}
					else
					{
						if(sym==beginsym)   /*׼�����ո�����䴦��*/
						{
							getsymdo;
							memcpy(nxtlev,fsys,sizeof(bool)*symnum);
							nxtlev[semicolon]=true;
							nxtlev[endsym]=true;/*�������Ϊ�ֺŻ�end*/
							/*ѭ��������䴦��������ֱ����һ�����Ų�����俪ʼ���Ż��յ�end*/
							statementdo(nxtlev,ptx,lev);
							while(inset(sym,statbegsys)||sym==semicolon)
							{
								if(sym==semicolon)
								{
									getsymdo;
								}
								else
								{
									error(10);/*ȱ�ٷֺ�*/
								}
								statementdo(nxtlev,ptx,lev);
							}
							if(sym==endsym)
							{
								getsymdo;
							}
							else
							{
								error(17); /*ȱ��end��ֺ�*/
							}
						}
						else
						{
							if(sym==whilesym)/*׼������while��䴦��*/
							{
								cx1=cx;        /*�����ж�����������λ��*/
								getsymdo;
								memcpy(nxtlev,fsys,sizeof(bool)*symnum);
								nxtlev[dosym]=true;/*�������Ϊdo*/
								conditiondo(nxtlev,ptx,lev);  /*������������*/
								cx2=cx;       /*����ѭ����Ľ�������һ��λ��*/
								gendo(jpc,0,0);/*����������ת��������ѭ���ĵ�ַδ֪*/
								if(sym==dosym)
								{
									getsymdo;
								}
								else
								{
									error(18);      /*ȱ��do*/
								}
								statementdo(fsys,ptx,lev); /*ѭ����*/
								gendo(jmp,0,cx1);/*��ͷ�����ж�����*/
								code[cx2].a=cx;   /*��������ѭ���ĵ�ַ����if����*/
							}
							else
							{
								memset(nxtlev,0,sizeof(bool)*symnum);/*�������޲��ȼ���*/
								testdo(fsys,nxtlev,19);/*�������������ȷ��*/
							}
						}
					}
				}
			}
		}
	}
	return 0;
}
/*
*����ʽ����
*/
int expression(bool*fsys,int*ptx,int lev)
{
	enum symbol addop;                    /*���ڱ���������*/
	bool nxtlev[symnum];
	if(sym==plus||sym==minus)             /*��ͷ�������ţ���ʱ��ǰ����ʽ������һ�����Ļ򸺵���*/
	{
		addop=sym;                    /*���濪ͷ��������*/
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);                /*������*/
		if(addop==minus)
		{
			gendo(opr,0,1);                   /*�����ͷΪ��������ȡ��ָ��*/
		}
	}
	else                             /*��ʱ����ʽ��������ļӼ�*/
	{
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);            /*������*/
	}
	while(sym==plus||sym==minus)
	{
		addop=sym;
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus]=true;
		nxtlev[minus]=true;
		termdo(nxtlev,ptx,lev);              /*������*/
		if(addop==plus)
		{
			gendo(opr,0,2);                    /*���ɼӷ�ָ��*/
		}
		else
		{
			 gendo(opr,0,3);                /*���ɼ���ָ��*/
		}
	}
	return 0;
}
/*
*���
*/
int term(bool*fsys,int *ptx,int lev)
{
    enum symbol mulop;               /*���ڱ���˳�������*/
    bool nxtlev[symnum];
    memcpy(nxtlev,fsys,sizeof(bool)*symnum) ;
    nxtlev[times]=true;
    nxtlev[slash]=true;
    factordo(nxtlev,ptx,lev);       /*��������*/
    while(sym==times||sym==slash)
    {
		mulop=sym;
        getsymdo;
        factordo(nxtlev,ptx,lev);
        if(mulop==times)
        {
            gendo(opr,0,4);          /*���ɳ˷�ָ��*/
        }
        else
        {
            gendo(opr,0,5);           /*���ɳ���ָ��*/
        }
    }
     return 0;
}
/*
*���Ӵ���
*/
int factor(bool*fsys,int *ptx,int lev)
 {
    int i;
    bool nxtlev[symnum];
    testdo(facbegsys,fsys,24);           /*������ӵĿ�ʼ���ú�*/
    while(inset(sym,facbegsys))          /*ѭ��ֱ���������ӿ�ʼ����*/
    {
        if(sym==ident)                   /*����Ϊ�������߱���*/
        {
            i=position(id,*ptx);        /*��������*/
            if(i==0)
            {
                error(11);               /*��ʶ��δ����*/
            }
            else
            {
				switch(table[i].kind)
				{
					case constant:                                      /*����Ϊ����*/
						gendo(lit,0,table[i].val);                       /*ֱ�Ӱѳ�����ֵ��ջ*/
						break;
					case variable:                                      /*����Ϊ����*/
						gendo(lod,lev-table[i].level,table[i].adr);      /*�ҵ�������ַ������ֵ��ջ*/
						break;
					case procedur:                                      /*����Ϊ����*/
						error(21);                                       /*����Ϊ����*/
						break;
				}
			}
			getsymdo;
		}
		else
		{
			if(sym==number)                                             /*����Ϊ��*/
			{
				if(num>amax)
				{
					error(31);
					num=0;
				}
				gendo(lit,0,num);
				getsymdo;
			}
			else
			{
				if(sym==lparen)                                           /*����Ϊ����ʽ*/
				{
					getsymdo;
					memcpy(nxtlev,fsys,sizeof(bool)*symnum);
					nxtlev[rparen]=true;
					expressiondo(nxtlev,ptx,lev);
					if(sym==rparen)
					{
						getsymdo;
					}
					else
					{
						error(22);                                       /*ȱ��������*/
					}
				}
				testdo(fsys,facbegsys,23);                        /*���Ӻ��зǷ�����*/
			}
		}
	}
	return 0;
}
/*
 ��������*/
int condition(bool* fsys,int* ptx,int lev)
{
    enum symbol relop;
    bool nxtlev[symnum];
    if(sym==oddsym)                        /*׼������odd���㴦��*/
   	{
		getsymdo;
		expressiondo(fsys,ptx,lev);
		gendo(opr,0,6);                              /*����oddָ��*/
    }
    else
    {
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[eql]=true;
		nxtlev[neq]=true;
		nxtlev[lss]=true;
		nxtlev[leq]=true;
		nxtlev[gtr]=true;
		nxtlev[geq]=true;
		expressiondo(nxtlev,ptx,lev);
		if(sym!=eql&&sym!=neq&&sym!=lss&&sym!=leq&&sym!=gtr&&sym!=geq)
		{
			error(20);
		}
		else
		{
			relop=sym;
			getsymdo;
			expressiondo(fsys,ptx,lev);
			switch(relop)
			{
				case eql:
					gendo(opr,0,8);
					break;
				case neq:
					gendo(opr,0,9);
					break;
				case lss:
					gendo(opr,0,10);
					break;
				case geq:
					gendo(opr,0,11);
					break;
				case gtr:
					gendo(opr,0,12);
					break;
				case leq:
					gendo(opr,0,13);
					break;
			}

		}
    }
    return 0;
}           
                                       
/*���ͳ���*/
void interpret()
{
	int p,b,t;    //ָ��ָ�룬ָ���ַ��ջ��ָ��    
	struct instruction i;  //��ŵ�ǰָ��    
	int s[stacksize];      //ջ    
	printf("start pl0\n");
	t=0;
	b=0;
	p=0;
	s[0]=s[1]=s[2]=0;
	
	int pi = 0, p_level = -1, pointer = 0;	//p_level��¼ָ�����ڲ�� 
	int array[10];//��¼��ǩ��������� 
	fprintf(res,"msg1:\t.asciiz  \"\\n\"\nmsg2:\t.asciiz \"Input a integer: \"\n.text\nmain:\n");

	/*��һ�飬����pl0ָ����� */
	for(int j=0;j<cx;++j){
		i=code[j];         /*����ǰָ��*/
		pi=pi+1;
		code[j].flag = pi - 1;	/*��ָ���ѱ����룬��¼ָ���к�*/
		switch(i.f)
		{
			case opr:        /*���֡��߼�����*/
				if(i.a==0){
					array[pointer++] = i.a;
					break;
				} 
				else if(i.a==6||i.a==14||i.a==15||i.a==16){
					pi += 2;					
					break;
				}
				else if(i.a==5){
					pi += 1;						
					break;
				}
				break;
			case jmp:             /*ֱ����ת*/
				array[pointer++] = i.a;			
				break;
			case jpc:              /*������ת*/
				array[pointer++] = i.a;	
	    		break;
		}
	}

	int array_p[10], array_b[10], array_t[10];//�ֱ��Ӧ����p,b,tָ��
	array_p[0] = 0;array_b[0] = 0;array_t[0] = 8;
	int p_regs = 8;   /*��ǰ�ļĴ�������pl0ջ�����Ӧ��*/

	//ָ��ת�� 
	do{
		i=code[p];         /*����ǰָ��*/
		p++;
		pi++;	/*pi�Ƕ�Ӧmipsָ����к�*/
		//д���ǩ 
		for(int j=0;j<pointer;j++){
			if(p-1==array[j]&&array[j]!= 0){
				fprintf(res, "label%d:\n",array[j]);
				array[j] = 0; 
				break;
			}
		}
		switch(i.f)
		{
			case lit:        /*��a��ֵȡ��ջ��*/
				s[t]=i.a;
				t=t+1;	
				if(i.flag != -1){		//��ʾ�ѷ���Ϊresָ��
					code[p-1].flag = -1;
					fprintf(res,"\tli  $%d  %d\n", p_regs++, i.a);//liu $1 10
				}	
				break;
			case opr:        /*���֡��߼�����*/
				switch(i.a)
				{
					case 0:
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs = array_t[p_level];		//�ص���һ�����õļĴ���
							p_level--;		//�ص���һ��
							if (p_level >= 0);
							else{
								fprintf(res,"\tj  exit\n");
							}
						}
						t=b;
						p=s[t+2];
						b=s[t+1];
						break;
					case 1:
						s[t-1]=-s[t-1];
						if(i.flag != -1){
							code[p-1].flag = -1;
							pi++;
							fprintf(res,"\tli  $%d  -1\t", p_regs);
							fprintf(res, "\tmul  $%d  $%d  $%d\n", p_regs-1, p_regs-1, p_regs); 
						}
						break;
					case 2:
						t--;
						s[t-1]=s[t-1]+s[t];
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;			
							fprintf(res,"\tadd  $%d  $%d  $%d\n", p_regs-1, p_regs-1, p_regs);
						}
						break;
					case 3:
						t--;
						s[t-1]=s[t-1]-s[t];
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;		
							fprintf(res,"\tsub  $%d  $%d  $%d\n", p_regs-1, p_regs-1, p_regs);
						}
						break;
					case 4:
						t--;
						s[t-1]=s[t-1]*s[t];
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;	
							fprintf(res,"\tmul  $%d  $%d  $%d\n", p_regs-1, p_regs-1, p_regs);
						}
						break;
					case 5:
						t--;
						s[t-1]=s[t-1]/s[t];
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;	
							pi++;
							fprintf(res,"\tdiv  $%d  $%d\n", p_regs-1, p_regs);
							fprintf(res,"\tmflo  $%d\n", p_regs-1);
						}
             			break;
					case 6:
						s[t-1]=s[t-1]%2;
						if(i.flag != -1){
							code[p-1].flag = -1;
							pi += 2;
							fprintf(res,"\taddi  $%d,  $0,  2\n", p_regs);
							fprintf(res,"\tdiv  $%d,  $%d\n", p_regs-1, p_regs);	
							fprintf(res,"\tmfhi  $%d\n", p_regs-1);	
						}
						break;
					case 8:
						t--;
						s[t-1]=(s[t-1]==s[t]);
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;	
							fprintf(res,"\tseq  $%d,  $%d,  $%d\n", p_regs-1, p_regs-1, p_regs);	
						}	
 						break;
					case 9:
						t--;
						s[t-1]=(s[t-1]!=s[t]);
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;	
							fprintf(res,"\tsne  $%d,  $%d,  $%d\n", p_regs-1, p_regs-1, p_regs);		
						}
 						break;
					case 10:
						t--;
						s[t-1]=(s[t-1]<s[t]);	
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;						 
							fprintf(res,"\tslt  $%d,  $%d,  $%d\n", p_regs-1, p_regs-1, p_regs);	
						}				
 						break;
					case 11:
						t--;
						s[t-1]=(s[t-1]>=s[t]);
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;	
							fprintf(res,"\tsge  $%d,  $%d,  $%d\n", p_regs-1, p_regs-1, p_regs);
						}
 						break;
					case 12:
						t--;
						s[t-1]=(s[t-1]>s[t]);
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;	
							fprintf(res,"\tsgt  $%d,  $%d,  $%d\n", p_regs-1, p_regs-1, p_regs);	
						}
 						break;
					case 13:
						t--;
						s[t-1]=(s[t-1]<=s[t]);
						if(i.flag != -1){
							code[p-1].flag = -1;
							p_regs--;	
							fprintf(res,"\tsle  $%d,  $%d,  $%d\n", p_regs-1, p_regs-1, p_regs);	
						}
 						break;
					case 14:
						printf("%d",s[t-1]);
						fprintf(fa2,"%d",s[t-1]);
						t--;
						if(i.flag != -1){
							code[p-1].flag = -1;
							//���ָ�� 
							pi += 2;
							fprintf(res,"\tmove  $a0,  $%d\n", p_regs-1);
							fprintf(res,"\tli  $v0,  1\n");
							fprintf(res,"\tsyscall\n");
							p_regs--;	
						}
						break;
					case 15:
						printf("\n");
						fprintf(fa2,"\n");
						if(i.flag != -1){
							code[p-1].flag = -1;
							//�������ָ�� 
							pi += 2;
							fprintf(res,"\tli  $v0,  4\n");
							fprintf(res,"\tla  $a0,  msg1\n");
							fprintf(res,"\tsyscall\n");	
						}
						break;
					case 16:
						printf("?");
						fprintf(fa2,"?");
						scanf("%d",&(s[t]));
						fprintf(fa2,"%d\n",s[t]);
						t=t+1;
						if(i.flag != -1){
							code[p-1].flag = -1;
							//����ָ�� 
							pi += 2;
							fprintf(res,"\tli  $v0,  5\n");
							fprintf(res,"\tsyscall\n");
							fprintf(res,"\tmove  $%d,  $v0\n",p_regs);
							p_regs++;	
		
						}					
						break;
				}
				break;
			case lod:       /*ȡ��Ե�ǰ���̵����ݻ���ַΪ����ڴ��ֵ��ջ��*/
				s[t]=s[base(i.l,s,b)+i.a];
				t++;
				if(i.flag != -1){
					code[p-1].flag = -1;
					if(p_level > 0)
						fprintf(res,"\tmove  $%d,  $%d\n",p_regs, array_t[p_level-i.l]+i.a-3);
					else
						fprintf(res,"\tmove  $%d,  $%d\n", p_regs, array_t[0]+i.a-3);
					p_regs++;
				}
				break;
			case sto:       /*ջ����ֵ�浽��Ե�ǰ���̵����ݻ���ַΪ����ڴ�*/
				t--;
	            s[base(i.l,s,b)+i.a]=s[t];
	            if(i.flag != -1){
					code[p-1].flag = -1;
					p_regs--;
					if(p_level > 0)
						fprintf(res,"\tmove  $%d,  $%d\n", array_t[p_level-i.l]+i.a-3, p_regs);
					else
						fprintf(res,"\tmove  $%d,  $%d\n", array_t[0]+i.a-3, p_regs);						
				}
				break;
			case cal:              /*�����ӳ���*/
				if(i.flag != -1){
					code[p-1].flag = -1;
					fprintf(res,"\tj  label%d\n", i.a);
				}	
				s[t]=base(i.l,s,b); /*�������̻���ַ��ջ*/
				s[t+1]=b;           /*�������̻���ַ��ջ������������base����*/
				s[t+2]=p;           /*����ǰָ��ָ����ջ*/
				b=t;                /*�ı����ַָ��ֵΪ�¹��̵Ļ���ַ*/
				p=i.a;              /*��ת*/
				break;
			case inte:             /*�����ڴ�*/
				t+=i.a;
				if(i.flag != -1){
					code[p-1].flag = -1;
					p_level++; 
					array_p[p_level] = pi;
					array_b[p_level] = pi;
					array_t[p_level] = p_regs;
					p_regs += i.a-3;
				}
				break;
			case jmp:             /*ֱ����ת*/
				if(i.flag != -1){
					code[p-1].flag = -1;					
					fprintf(res,"\tj  label%d\n", i.a);
				}
				p=i.a;
				break;
			case jpc:              /*������ת*/
				if(i.flag != -1){
					code[p-1].flag = -1;
					p_regs--;	
					fprintf(res,"\tbeq  $%d, $0, label%d\n", p_regs, i.a);
				}
				t--;
				if(s[t]==0)
				{
					p=i.a;
				}
				
	    		break;
		}
		
		// �Ĵ������������
		if(p_regs >= 25){
			printf("error,register is not enough!\n");
			break;
		} 
	}while (p!=0);
	
	/* �����˳� */
	fprintf(res,"exit:\n");
	fprintf(res,"\tli  $v0, 10\n"); 
	fprintf(res,"\tsyscall");	

}
/*ͨ�����̻�ַ����1����̵Ļ�ַ*/
int base(int l,int * s,int b)
{
	int b1;
	b1=b;
	 while(l>0)
	 {
		 b1=s[b1];
     	 l--;
	 }
	 return b1;
}