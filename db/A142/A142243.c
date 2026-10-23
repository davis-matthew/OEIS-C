/*
 * A142243  Triangle T(n,k) = binomial(2*n,k) *binomial(2*n-2*k,n-k), read by
 *          rows; 0<=k<=n.
 * https://oeis.org/A142243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) = binomial(2*n,k) * binomial(2*n-2*k,n-k).  The sequence
   a(n) is obtained by reading the triangle row‑by‑row, summing each row
   element in order.  For a given index n, let r = floor((sqrt(8*n+1)-1)/2)
   and k = n - r*(r+1)/2; then a(n) = binomial(2*r,k) * binomial(2*r-2*k,
   r-k).  */
#include <stdint.h>
#include <math.h>

/* Compute binomial(n,k) for idx_t arguments using an iterative multiplicative
   formula.  The product is performed in seq_t to avoid overflow of idx_t.  */
static inline seq_t binom_simple(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r: r satisfies r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t t = (idx_t)sqrt((long double)(8 * n + 1));
    idx_t r = (t - 1) / 2;

    /* Column index k within the row */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    seq_t first  = binom_simple(2 * r, k);
    seq_t second = binom_simple(2 * r - 2 * k, r - k);
    return first * second;
}
