/*
 * A015525  Expansion of x/(1-3*x-8*x^2).
 * https://oeis.org/A015525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;  /* a(0) */
    seq_t prev1 = 1;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = 3 * prev1 + 8 * prev2;  /* compute a(i) */
        prev2 = prev1;                      /* shift for next iteration */
        prev1 = cur;
    }
    return prev1;  /* a(n) */
}
