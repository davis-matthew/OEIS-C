/*
 * A021448  Decimal expansion of 1/444.
 * https://oeis.org/A021448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 2*((n+1) mod 3) + (n mod 3)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;
    /* modulo 3 cycles the pattern 2,2,5 */
    idx_t r = n % 3;
    return (r == 1) ? (seq_t)5 : (seq_t)2;
}
