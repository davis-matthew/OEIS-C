/*
 * A047226  Numbers that are congruent to {0, 1, 2, 3, 4} mod 6;
 *          a(n)=floor(6(n-1)/5).
 * https://oeis.org/A047226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) + floor((n-1)/5)
 * Range:     u8 n <= 214, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = floor(6*(n-1)/5)     */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behavior for n=0 is arbitrary */
        return (seq_t)0;

    idx_t s = n - 1;                 /* s = n-1  */
    seq_t res = (seq_t)s + (seq_t)(s / 5);  /* s + floor(s/5)  */
    return res;
}
