/*
 * A172281  Triangle T(n, k, q) = ceiling( binomial(n, k)*(1+q)/( (1+q)^2 + (k
 *          - floor(n/2))^2 ) ), with q = 1, read by rows.
 * https://oeis.org/A172281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) = ⌈ binomial(row,k)*2 / (4 + (k - ⌊row/2⌋)^2) ⌉, where row
 *            and k are obtained from the index n by the triangular-number
 *            transformation
 * Range:     u8 n <= 58, u16 n <= 217, u32 n <= 495, u64 n <= 495,
 *            u128 n <= 495, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast multiplicative computation of binomial coefficient
   with idx_t arguments and seq_t result */
static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
        result = result * (n - k + i) / i;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find the row r such that triangular number Tri(r-1) <= n < Tri(r)
       Tri(r) = (r+1)(r+2)/2.  We increment r while the next triangular
       number is <= n.  This is O(√n) and safe for n up to 10^8. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* starting index of this row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* position inside the row */

    /* compute binomial coefficient binom(r, k) */
    seq_t m = binomial_multiplicative(r, k);

    /* numerator = m * 2 */
    seq_t numerator = m * 2;

    /* compute denominator = 4 + (k - floor(r/2))^2 */
    long long diff = (long long)k - (long long)(r / 2);
    seq_t diff_sq = (seq_t)(diff * diff);
    seq_t denominator = 4 + diff_sq;

    /* ceiling of numerator / denominator */
    seq_t q = numerator / denominator;
    seq_t rmd = numerator % denominator;
    if (rmd != 0)
        ++q;
    return q;
}
