/*
 * A111516  Triangle G(n,k) read by rows: number of order-preserving partial
 *          transformations (of an n-element totally ordered set) of waist k
 *          (waist(alpha) = max(Im(alpha))).
 * https://oeis.org/A111516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   G(n,k) = Sum_{j=1..n} C(n,j)*C(k+j-2,j-1) for 1 ≤ k ≤ n, G(n,0)
 *            = 1. a(n) = G(row, col) where row = row_of(n) and col = n -
 *            row*(row+1)/2. (row_of defined in utils/row_of.h)
 * Range:     u8 n <= 18, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t
row_of(idx_t n)                 /* return r with r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
{
    idx_t low  = 0;
    idx_t high = 1;
    /* exponential search to find an upper bound */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low  = high;
        high <<= 1;              /* double high */
    }
    /* binary search between low (inclusive) and high (inclusive) */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1) + 1; /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Find the row (r) and column (k) in the triangle for index n. */
    idx_t r = row_of(n);
    idx_t k = n - r * (r + 1) / 2;

    /* G(r,0) = 1 */
    if (k == 0) {
        return (seq_t)1;
    }

    /* Compute the sum ∑_{j=1}^{r} C(r,j) * C(k+j-2,j-1). */
    seq_t sum = 0;
    for (idx_t j = 1; j <= r; ++j) {
        sum += binomial(r, j) * binomial(k + j - 2, j - 1);
    }
    return sum;
}
