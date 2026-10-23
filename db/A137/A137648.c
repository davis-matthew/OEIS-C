/*
 * A137648  a(n) = Sum_{k=0..n} C(k^2, k) * C(k^2, n-k).
 * https://oeis.org/A137648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C(k², k) · C(k², n‑k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* k² fits in idx_t for all intended n (≤ 10 000 gives k²≤10⁸) */
        idx_t m = k * k;

        seq_t b1 = binom(m, k);
        seq_t b2 = binom(m, n - k);

        sum += b1 * b2;
    }
    return sum;
}
