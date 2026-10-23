/*
 * A124279  Riordan array (1/(1-x),x(1-x+x^2)/(1-x)).
 * https://oeis.org/A124279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row and k is the column index such
 *            that * start_of_row(r) ≤ n < start_of_row(r+1) * and T(r,k) =
 *            Σ_{j=0}^{r-k} C(j, r-k-j) · C(k, r-k-j) * (binomial coefficients
 *            C(a,b) are computed by the provided `binomial` function)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Compute the starting linear index of row r: r*(r+1)/2 */
static inline idx_t start_of_row(idx_t r)
{
    return (idx_t)(((unsigned __int128)r * (r + 1)) >> 1);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the row r containing index n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (start_of_row(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t start = start_of_row(r);
    idx_t k   = n - start;          /* column within the row */
    idx_t m   = r - k;              /* upper limit for the sum */

    seq_t sum = 0;
    for (idx_t t = 0; t <= m; ++t) {
        seq_t term1 = binomial(m - t, t);
        seq_t term2 = binomial(k, t);
        sum += term1 * term2;
    }
    return sum;
}
