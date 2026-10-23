/*
 * A141723  Triangle T(n, k) = Sum_{j=0..n} (2*n)!/((2*n-k-j)!*j!*k!), read by
 *          rows.
 * https://oeis.org/A141723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = row number such that * r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2, * k = n – r*(r+1)/2, * and T(r,k) = Σ_{j=0..r}
 *            (2r)! / ((2r-k-j)! ⋅ j! ⋅ k!) * = binomial(2r,k) * Σ_{j=0..r}
 *            binomial(2r‑k, j)
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 71, u64 n <= 261,
 *            u128 n <= 924, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Determine the row r and column k of the (r,k)-entry that is
       encountered at the linear index n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)   /* find largest r with r*(r+1)/2 ≤ n */
        ++r;
    idx_t k = n - (r * (r + 1) / 2);       /* offset within the row */

    /* Compute binomial(2r,k). */
    idx_t double_r = 2 * r;
    seq_t binom_2r_k = binomial(double_r, k);

    /* Compute the sum Σ_{j=0..r} binomial(2r‑k, j). */
    idx_t m = double_r - k;                /* m = 2r - k */
    seq_t sum  = 0;
    seq_t coeff = 1;                       /* binomial(m,0) */
    sum = coeff;                           /* j = 0 */
    for (idx_t j = 1; j <= r; ++j) {
        /* compute next binomial coefficient multiplicatively: */
        coeff = (coeff * (seq_t)(m - j + 1)) / (seq_t)j;
        sum += coeff;
    }

    /* Final result.  For seq_t larger than 64 bits this uses arbitrary-precision
       arithmetic; for 64-bit seq_t values that fit the result is exact. */
    return binom_2r_k * sum;
}
