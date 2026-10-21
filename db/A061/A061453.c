/*
 * A061453  a(n) = numerator of the continued fraction [1; 2^2, 3^3, ...,
 *          n^n].
 * https://oeis.org/A061453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = n^n * a(n-1) + a(n-2) with a(1)=1 and a(2)=5
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   pow_seq_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq_i.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 5;
    seq_t prev2 = 1;   /* a(1) */
    seq_t prev1 = 5;   /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t p = pow_seq_i((seq_t)i, i);
        seq_t cur = p * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
