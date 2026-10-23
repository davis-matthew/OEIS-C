/*
 * A010368  Number of points of L1 norm 2n in Hamming code version of E_8
 *          lattice.
 * https://oeis.org/A010368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{w∈{0,4,8}} A_w ⋅ 2^w ⋅ Σ_{j=0}^{min(8-w, n-w/2)}
 *            binom(8-w, j) ⋅ binom(7 + n - w/2 - j, 7) where A₀=1, A₄=14,
 *            A₈=1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 24, u64 n <= 582, u128 n <= 1000,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* w = 0 term (A₀ = 1) */
    for (idx_t j = 0; j <= 8 && j <= n; ++j) {
        seq_t term = binom(8, j) * binom(7 + (n - j), 7);
        res += term;
    }

    /* w = 4 term (A₄ = 14, 2⁴ = 16, 14·16 = 224) */
    if (n >= 2) {
        idx_t R = n - 2;           /* R = n - w/2 */
        for (idx_t j = 0; j <= 4 && j <= R; ++j) {
            seq_t term = binom(4, j) * binom(7 + (R - j), 7);
            res += (seq_t)224 * term;
        }
    }

    /* w = 8 term (A₈ = 1, 2⁸ = 256) */
    if (n >= 4) {
        seq_t term = binom(7 + (n - 4), 7); /* binom(n+3, 7) */
        res += (seq_t)256 * term;
    }

    return res;
}
