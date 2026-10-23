/*
 * A163459  a(n) = 14*a(n-1) - 47*a(n-2) for n > 1; a(0) = 1, a(1) = 8.
 * https://oeis.org/A163459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 47*a(n-2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    /* Iterate using two previous terms */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * 14 - prev2 * 47;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
