/**********************************************************************
*
* © James D. Broesch, 2011, 2013, ... 2021
* Correlation Detector
* This code demonstrates the use of mixed programming techniques.
* A vector MAC routine, written in assembly, is called from the C code.
*
* FileName:        main.c
* Dependencies:    Header (.h) files if applicable, see below
* Processor:       dsPIC33FJ256GP506
* Compiler:        MPLAB® C30 v3.00 or higher
*
************************************************************************/
#include <math.h>

#include "p33FJ256GP506.h"
#include "WM8510CODEC.h"
#include "timer.h"
#include "dscInit.h"

// Declare the vector funcions as external.
extern int vectorMAC(int *x, int *y, unsigned int loop_count,unsigned int scale);
extern int vectorShift(int *vector, int vectorLength);

extern unsigned int dataReady;
 
#define SIGNAL_SIZE 100
#define PADDING 20

signed short int signalIn;
signed short int signalOut;
signed short int sigma[20];

// The signal vector is placed in the x memory space.
//The coefficents are placed in the y memory space.
//This allows single cycle MAC operations.
signed int signal[SIGNAL_SIZE + PADDING +1] __attribute__((space(xmemory), aligned,));
signed int reference[SIGNAL_SIZE + 1]__attribute__ ((space(ymemory), aligned));

int main(void)
{	
	short unsigned int n; // Working variable.
	short signed int temp;
	
	// Definitions for building the coefficents.
	float f = (float)5; // Number of cycles in the vector.
	float T = (float)1/(float)SIGNAL_SIZE;
	float PI =  3.14159265;

	// Intialize the board and the drivers
	DSCInit();

	// Initalize the CODEC.
	WM8510Init();

	// Start Audio input and output function
	WM8510Start();
		
	// Configure the CODEC for 8K operation	
	WM8510SampleRate8KConfig();

	for (n =0; n < SIGNAL_SIZE +PADDING; n++)
	{
		signal[n] = 0;
	}

	// Generate the reference waveform.
	for (n =0; n < SIGNAL_SIZE; n++)
	{
		reference[n] = sin(2 * PI * f *(float)n * T) * 0x7FFF;
	}

	// Clear the correlation result vector.
	for (n =0; n < PADDING; n++)
	{
		sigma[n] = 0;
	}

	// Use timer 1 as a real time clock.
	// 1 tick = 15,625 * 256 clock cycles = 100ms.
	timer1Init(15625);

	GREEN_LED = SASK_LED_OFF;

	// Main processing loop.
	// Quit if switch 1 is pressed.
	while(SWITCH_S1)
	{
		if (dataReady)
		{
			dataReady = 0;
		

			// Your code goes here.


			temp = 0;
			for (n = 0; n < PADDING; n++)
				if (sigma[n] > temp) temp = sigma[n];
			
			// Set your threshold value. 
			if (temp > 5000) GREEN_LED = SASK_LED_ON;
			else GREEN_LED = SASK_LED_OFF;

			// Timing trap.
			if (dataReady)
					n = 0; // Set a breakpoint here.
	
		}
	} 

return(0); // Go home.
}
