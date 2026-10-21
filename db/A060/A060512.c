/*
 * A060512  Number of units of time making up the next larger unit: seconds,
 *          minutes, hours, days, weeks, years, centuries, millennia.
 * https://oeis.org/A060512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   a(1)=60, a(2)=60, a(3)=24, a(4)=7, a(5)=52, a(6)=100, a(7)=10 *
 *            The numbers are the standard conversion factors between
 *            successive units of * time: seconds→minutes, minutes→hours,
 *            hours→days, days→weeks, * weeks→years, years→centuries,
 *            centuries→millennia
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t table[] = { 60, 60, 24, 7, 52, 100, 10 };
    if (n >= 1 && n <= sizeof(table) / sizeof(table[0]))
        return table[n - 1];
    return (seq_t)0;
}
