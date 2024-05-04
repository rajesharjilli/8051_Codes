#include<reg52.h>



/**relay connections****/
sbit L1_in1 = P3^3;
sbit L1_in2 = P3^4;
sbit L1_in3 = P3^5;
sbit L1_in4 = P3^6;

/**L293D1 on board connections****/
sbit L2_in1 = P2^4;
sbit L2_in2 = P2^5;
sbit L2_in3 = P2^6;
sbit L2_in4 = P2^7;

sbit pump = P0^7;

				
void delay(unsigned int value);

unsigned char key;

void Ext_int_Init()				
{
	EA  = 1;		/* Enable global interrupt */
	ES = 1;  		/* Enable serial interrupt */			
}

void UART_Init()
{
	TMOD = 0x20;		/* Timer 1, 8-bit auto reload mode */
	TH1 = 0xFD;		/* Load value for 9600 baud rate */
	SCON = 0x50;		/* Mode 1, reception enable */
	TR1 = 1;		/* Start timer 1 */
}

void Transmit_data(char tx_data)
{
	SBUF = tx_data;		/* Load char in SBUF register */
	while (TI==0);		/* Wait until stop bit transmit */
	TI = 0;			/* Clear TI flag */
}

void uart_puts(char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)	/* Send each char of string till the NULL */
	{
		Transmit_data(str[i]);	/* Call transmit data function */
	}
}

void Serial_ISR() interrupt 4    
{
	char key = SBUF;		/* Give received data on port 1 */
	RI = 0;			/* Clear RI flag */
	if(key=='1')	                //L1 robot forward
    {
	  uart_puts("\n\r forward");
      L1_in1 = 1;
      L1_in2 = 0;
      L1_in3 = 1;
      L1_in4 = 0;
	}
    
   if(key=='2')		   //L1 robot backward
   {
      uart_puts("\n\r backward");

      L1_in1 = 0;
      L1_in2 = 1;
      L1_in3 = 0;
      L1_in4 = 1;
   }
    
   if(key=='3')		  //robot right
   {
	  uart_puts("\n\r Right");

      L1_in1 = 1;
      L1_in2 = 0;
      L1_in3 = 0;
      L1_in4 = 1;

   }

   if(key=='4')		  //robot left
   {
	  uart_puts("\n\r Left");

      L1_in1 = 0;
      L1_in2 = 1;
      L1_in3 = 1;
      L1_in4 = 0;
	 
   }
      if(key=='6')		  //L2 UP
   {
      uart_puts("\n\r UP");
	  L2_in1 = 0;
      L2_in2 = 1;
	  delay(500);
      L2_in1 = 0;
      L2_in2 = 0;
   }

   if(key=='7')		  //L2 DOWN
   {
      uart_puts("\n\r Down");
	  L2_in1 = 1;
      L2_in2 = 0;
	  delay(500);
      L2_in1 = 0;
      L2_in2 = 0;			 
   }

   if(key=='8')		  //L2 Cutter On
   {
     uart_puts("\n\r Cutter On");
	  L2_in3 = 1;
      L2_in4 = 0;
	  
   }

   if(key=='9')		  //L2 Cutter Off
   {
      uart_puts("\n\r Cutter Off");
	  L2_in3 = 0;
      L2_in4 = 0;
   }

      if(key=='w')		  //L2 Water On
   {
      uart_puts("\n\r Water on");
      pump = 1;
   }

     if((key=='s') || (key == '5'))		  //L2 Water Off
   {
      uart_puts("\n\r Water off");
      pump = 0;
   }

   if(key=='5')		 //all 32stop
   {
      uart_puts("\n\r Stop");

      L1_in1 = 0;
      L1_in2 = 0;
      L1_in3 = 0;
      L1_in4 = 0;
	  L2_in1 = 0;
      L2_in2 = 0;
      L2_in3 = 0;
      L2_in4 = 0;

   }

}





void main()
{
      L1_in1 = 0;
      L1_in2 = 0;
      L1_in3 = 0;
      L1_in4 = 0;

	  L2_in1 = 0;
      L2_in2 = 0;
      L2_in3 = 0;
      L2_in4 = 0;


	  uart_puts("1-front, 2-back, 3-left, 4-right, 5-stop");
	  uart_puts("6-up,7-Down,8-Cutter On,9-Cutter Off,w-water on, s-water stop");
	  pump = 0;

	Ext_int_Init();
	UART_Init();		/* UART initialize function */	 	 
	delay(1);
	uart_puts("Welcome ");	 Transmit_data('\n');
	while(1)
	{
	uart_puts("Send command \n\r"); Transmit_data('\n'); delay(100);
	}	
 } 

void delay(unsigned int value)
{
 unsigned int x,y;
 for(x=0;x<200;x++)
 for(y=0;y<value;y++);
}