/*
 * A099037  Triangle of diagonals of symmetric Krawtchouk matrices.
 * https://oeis.org/A099037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(n_row, n_col) where T(r,k) = binomial(r, k) *
 *            Σ_{i=0..k} (-1)^i * binomial(k,i) * binomial(r-k, k-i) (OEIS
 *            formula) (flattened row‑wise: a(0)=T(0,0), a(1)=T(1,0),
 *            a(2)=T(1,1), a(3)=T(2,0), ...)
 * Range:     i8 n <= 38, i16 n <= 96, i32 n <= 310, i64 n <= 1148,
 *            i128 n <= 4222, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"  /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t N)
{
    /* Determine row r and column c such that
       N = r*(r+1)/2 + c,  with 0 ≤ c ≤ r. */
    idx_t low = 0, high = N, r = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri <= N) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    idx_t c = N - r * (r + 1) / 2;

    /* Compute T(r,c) = binomial(r,c) * Σ_{i=0..c} (-1)^i * binomial(c,i) *
                                               binomial(r-c, c-i). */
    seq_t sum = 0;
    for (idx_t i = 0; i <= c; ++i) {
        seq_t bc_i = binomial(c, i);
        seq_t bc_rem = binomial(r - c, c - i);
        seq_t term = bc_i * bc_rem;
        if (i & 1U)
            sum -= term;
        else
            sum += term;
    }
    seq_t result = binomial(r, c) * sum;
    return result;
}
