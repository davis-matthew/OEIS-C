/*
 * A203092  Triangular array read by rows. T(n,k) is the number of partial
 *          functions on {1,2,...,n} that are endofunctions with no cycles of
 *          length > 1 that have exactly k components.
 * https://oeis.org/A203092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2) k = n -
 *            r(r+1)/2 and T(r,k) = Σ_{j=k}^{r} binom(r,j) * binom(j-1,k-1) *
 *            j^(j-k) with the convention T(r,0)=1. (For k>0 the above
 *            summation is used; for k==0 the value is 1.)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* find the row r and column k such that a(n) = T(r,k) */
    idx_t r = 0;
    /* cumulative count up to row r: (r+1)*(r+2)/2 terms */
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    /* T(r,0) = 1 */
    if (k == 0) {
        return (seq_t)1;
    }

    seq_t sum = 0;

    for (idx_t j = k; j <= r; ++j) {
        seq_t term = binom(r, j);                    /* C(r,j) */
        term *= binom(j - 1, k - 1);                 /* C(j-1,k-1) */
        term *= pow_int((seq_t)j, j - k);            /* j^(j-k) */
        sum += term;
    }

    return sum;
}
