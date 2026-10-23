/*
 * A008379  Crystal ball sequence for D_10 lattice.
 * https://oeis.org/A008379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0}^{10} c_k * C(n + 10 - k, 10) where c = [1, 170,
 *            4685, 38200, 124850, 183356, 124850, 38200, 4685, 170, 1]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 10, u64 n <= 102, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const int coeffs[11] = {
        1, 170, 4685, 38200, 124850, 183356,
        124850, 38200, 4685, 170, 1
    };
    seq_t sum = 0;
    for (idx_t k = 0; k <= 10 && k <= n; ++k) {
        seq_t bin = binom(n - k + 10, 10);
        seq_t term = bin * (seq_t)coeffs[k];
        sum += term;
    }
    return sum;
}
