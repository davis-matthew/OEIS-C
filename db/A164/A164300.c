/*
 * A164300  a(n) = ((1+4*sqrt(2))*(4+sqrt(2))^n +
 *          (1-4*sqrt(2))*(4-sqrt(2))^n)/2.
 * https://oeis.org/A164300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2); a(0)=1, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    /* Use two-argument recurrence, iteratively */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)12;  /* a(1) */
    seq_t val;

    for (idx_t i = 2; i <= n; ++i) {
        val = (seq_t)8 * a1 - (seq_t)14 * a0;
        a0 = a1;
        a1 = val;
    }
    return a1;
}
