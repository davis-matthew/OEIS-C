/*
 * A078369  A Chebyshev T-sequence with Diophantine property.
 * https://oeis.org/A078369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - a(n-2), a(0) = 2, a(1) = 19
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two initial values directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)19;

    /* Iteratively compute the recurrence up to n */
    seq_t a_prev2 = (seq_t)2;   /* a(0) */
    seq_t a_prev1 = (seq_t)19;  /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 19*a(i-1) - a(i-2) */
        a_curr = (seq_t)19 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
