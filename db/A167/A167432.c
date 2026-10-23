/*
 * A167432  Riordan array (c(2x)^2,xc(2x)), c(x) the g.f. of A000108.
 * https://oeis.org/A167432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 2^(r-k) * ((k+2)/(2r-k+2)) * binom(2r-k+2, r-k) where r
 *            is the row number given by the largest r with r(r+1)/2 ≤ n, and
 *            k = n - r(r+1)/2. The function uses the binom and pow2 utilities
 *            provided by the build
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Position k within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Auxiliary variables for the formula */
    idx_t N = 2 * r - k + 2;          /* denominator and binomial upper index */
    idx_t K = r - k;                  /* binomial lower index */

    /* Compute A054445(r,k) = (k+2)/(2r-k+2) * binom(2r-k+2, r-k) */
    seq_t res = binom(N, K);          /* binomial coefficient */
    res *= (k + 2);                   /* multiply by (k+2) */
    res /= N;                         /* exact division */

    /* Multiply by the power of 2: 2^(r-k) */
    res *= pow2(r - k);

    return res;
}
