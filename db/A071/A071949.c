/*
 * A071949  Triangle read by rows of numbers of paths in a lattice satisfying
 *          certain conditions.
 * https://oeis.org/A071949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r, k) where r = ⌊(√(8·n + 1) – 1)/2⌋, k = n – r·(r +
 *            1)/2, and T(r,0) = 1, for 0 < k ≤ r: T(r,k) = ((r – k + 1)/k) ⋅
 *            Σ_{j=0}^{k-1} 2^{j+1} ⋅ C(k, j+1) ⋅ C(r + k, j). (C(.,.) are
 *            binomial coefficients)
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column k from the index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    if (k == 0) {
        return (seq_t)1;
    }

    /* Compute the summation part of the formula */
    seq_t sum   = 0;
    seq_t pow2  = (seq_t)2;          /* 2^(1) for j = 0 */
    for (idx_t j = 0; j < k; ++j) {
        idx_t j1   = j + 1;
        seq_t term = pow2;
        term = term * binom(k, j1);
        term = term * binom(r + k, j);
        sum += term;
        pow2 = pow2 + pow2;          /* multiply by 2 for the next j */
    }

    seq_t coeff = (seq_t)(r - k + 1);
    seq_t res   = coeff * sum / (seq_t)k;
    return res;
}
