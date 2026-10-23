/*
 * A158687  Riordan array (1/(1-x),x(1+x)^2/(1-x)).
 * https://oeis.org/A158687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((sqrt(8*n+1)-1)/2), c = n -
 *            r*(r+1)/2, T(n,k) = Sum_{j=0..n-k} binom(n-j,k) * binom(2*k,j)
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The binomial coefficient routine is provided by the framework. */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r and column c corresponding to the linear index n. */
    idx_t r = 0;
    /* r*(r+1)/2 increases roughly as r^2/2, so a few dozen iterations suffice. */
    while ((seq_t)r * (r + 1) / 2 <= (seq_t)n)
        ++r;
    --r;
    seq_t row_start = (seq_t)r * (r + 1) / 2;
    idx_t c = n - (idx_t)row_start;            /* column index in the row */

    /* Compute the triangle entry T(r,c) = sum_{j=0}^{r-c} C(r-j,c)*C(2c,j). */
    seq_t result = 0;
    idx_t max_j = r - c;
    for (idx_t j = 0; j <= max_j; ++j)
    {
        seq_t term = binom(r - j, c) * binom(2 * c, j);
        result += term;
    }
    return result;
}
