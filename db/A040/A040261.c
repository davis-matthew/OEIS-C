/*
 * A040261  Continued fraction for sqrt(278).
 * https://oeis.org/A040261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040261:  Multiplicative, a(0)=16; for n>0, 
   a(n)= 2 if n divisible by 2, 16 if n divisible by 3,
   and 1 otherwise.  Thus a(n)= 2^{[2|n]} * 16^{[3|n]}. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)16;          /* offset 0 as specified */

    seq_t result = 1;

    if ((n % 2) == 0)
        result *= (seq_t)2;
    if ((n % 3) == 0)
        result *= (seq_t)16;

    return result;
}
