/*
 * A190995  Fibonacci sequence beginning 9, 7.
 * https://oeis.org/A190995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 9, a(1) = 7
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)9;
    if (n == (idx_t)1)
        return (seq_t)7;

    seq_t prev2 = (seq_t)9;   /* a(0) */
    seq_t prev1 = (seq_t)7;   /* a(1) */
    seq_t cur   = 0;

    idx_t i;
    for (i = (idx_t)2; i <= n; ++i) {
        cur   = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
