/*
 * A189741  a(1)=4, a(2)=2, a(n) = 4*a(n-1) + 2*a(n-2).
 * https://oeis.org/A189741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2), a(1) = 4, a(2) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle trivial cases directly */
    if (n == 1)
        return (seq_t)4;
    if (n == 2)
        return (seq_t)2;

    /* iteratively compute values using only the two previous terms */
    seq_t a_prev2 = (seq_t)4;   /* a(1) */
    seq_t a_prev1 = (seq_t)2;   /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i)
    {
        a_curr = (seq_t)4 * a_prev1 + (seq_t)2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}
