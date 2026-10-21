/*
 * A061551  Number of paths along a corridor width 8, starting from one side.
 * https://oeis.org/A061551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - a(n-4), with a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 71, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the first four values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* initialise the last four computed terms:
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1)      */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2 = (seq_t)2; /* a(2) */
    seq_t a3 = (seq_t)3; /* a(3) */
    seq_t a4;            /* a(n) to be computed */

    /* iterate from 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        a4 = a3 + (seq_t)3 * a2 - (seq_t)2 * a1 - a0;
        /* shift the window forward for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }

    return a4;
}
