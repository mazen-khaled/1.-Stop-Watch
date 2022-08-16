#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char sec=0,min=0,hours=0;

void PORT_init(void);
void INT0_init(void);
void INT1_init(void);
void INT2_init(void);
void TIMER1_init(void);

/*Reset the stopwatch*/
ISR(INT0_vect)
{
	sec=0;
	min=0;
	hours=0;
}

/*Pause the stopwatch*/
ISR(INT1_vect)
{
	   TCCR1B=0;
}

/*Resume the stopwatch*/
ISR(INT2_vect)
{
	TCCR1B =(1<<WGM12)|(1<<CS10)|(1<<CS12);
}

/*Stopwatch*/
ISR(TIMER1_COMPA_vect)
{
	sec++;
	if(sec==60)
	{
		sec=0;
		min++;
	}
	if(min==60)
	{
		min=0;
		hours++;
	}
}

int main (void)
{
	SREG|=(1<<7);
	PORT_init();
	INT0_init();
	INT1_init();
	INT2_init();
	TIMER1_init();

	while (1)
	{
		//enable First 7-segment
		PORTA =(1);
		PORTC = (PORTC & (0xF0)) | (sec % 10);
		_delay_us(20);

		//enable second 7-segment
		PORTA = (1<<1);
		PORTC = (PORTC & (0xF0)) | (sec/10);
		_delay_us(20);

		//enable third 7-segment
		PORTA = (1<<2);
		PORTC = (PORTC & (0xF0)) | (min % 10);
		_delay_us(20);

		//enable forth 7-segment
		PORTA = (1<<3);
		PORTC = (PORTC & (0xF0)) | (min/10);
		_delay_us(20);

		 //enable fifth 7-segment
		PORTA = (1<<4);
		PORTC = (PORTC & (0xF0)) | (hours %10);
		_delay_us(20);

		//enable sixth 7-segment
		PORTA = (1<<5);
		PORTC = (PORTC & (0xF0)) | (hours/10);
		_delay_us(20);
	}
}
void PORT_init(void)
{
	DDRC |=(0x0F);
	PORTC &=~(0x0F);

	DDRA |=(0x3F);
	PORTA &=~(0x3F);
}

/*Interrupt-0 as input*/
void INT0_init(void)
{
	DDRD &=~(1<<PD2);
	PORTD |=(1<<PD2);
	GICR |=(1<<INT0);
	//falling edge triggered
	MCUCR |=(1<<ISC01);
}

/*Interrupt-1 as input*/
void INT1_init(void)
{
	DDRD &=~(1<<PD3);
	GICR |=(1<<INT1);
	//rising edge triggered
	MCUCR = MCUCR |(1<<ISC10)|(1<<ISC11);
}

/*Interrupt-2 as input*/
void INT2_init(void)
{
	DDRB &=~(1<<PB2);
	PORTB |=(1<<PB2);
	GICR |=(1<<INT2);
	//falling edge triggered
	MCUCSR &=~(1<<ISC2);
}

//Timer settings
void TIMER1_init(void)
{
	TCNT1=0;
	TCCR1A =(1<<FOC1A);
	TCCR1B =(1<<WGM12)|(1<<CS10)|(1<<CS12);
	TIMSK |=(1<<OCIE1A);
	OCR1A=800;
}
