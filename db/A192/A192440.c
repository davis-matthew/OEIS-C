/*
 * A192440  Coefficient of x^floor(n/2) in the expansion of (1+x+x^3)^n.
 * https://oeis.org/A192440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊⌊n/2⌋/3⌋} binom(n, k) * binom(n-k, ⌊n/2⌋-3k)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 110,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* floor(n/2) */
    idx_t m = n / 2;
    /* maximum k such that m-3k ≥ 0 */
    idx_t kmax = m / 3;

    seq_t sum = 0;

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t j = m - 3 * k;               /* amount of x's */
        seq_t term = binom(n, k) * binom(n - k, j);
        sum += term;
    }

    return sum;
}
