/*
 * A015449  Expansion of (1-4*x)/(1-5*x-x^2).
 * https://oeis.org/A015449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for idx_t, seq_t are defined by the build */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t cur   = prev1;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = 5 * prev1 + prev2;  /* a(i) = 5*a(i-1)+a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
