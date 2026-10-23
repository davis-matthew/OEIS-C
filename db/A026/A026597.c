/*
 * A026597  Expansion of (1+x)/(1-x-4*x^2).
 * https://oeis.org/A026597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) with a(0) = 1, a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 2;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 + (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
