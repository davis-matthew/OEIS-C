/*
 * A155198  a(n)=9*a(n-1)+a(n-2), n>2 ; a(0)=1, a(1)=8, a(2)=72 .
 * https://oeis.org/A155198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + a(n-2) for n>2 Initial values: a(0)=1, a(1)=8,
 *            a(2)=72
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)72;

    /* a(2) and a(1) are the two previous terms before the loop starts */
    seq_t prevPrev = (seq_t)8;   /* a(1) */
    seq_t prev     = (seq_t)72;  /* a(2) */
    seq_t curr;

    /* Recurrence: a(i) = 9*a(i-1) + a(i-2) for i >= 3 */
    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)9 * prev + prevPrev;
        prevPrev = prev;
        prev = curr;
    }
    return prev;
}
