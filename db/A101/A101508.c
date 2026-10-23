/*
 * A101508  Product of binomial matrix and the Mobius matrix A051731.
 * https://oeis.org/A101508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where r = floor((√(8*n + 1) - 1)/2), c = n -
 *            r*(r+1)/2. T(r,c) = Σ_{m≥1, m*(c+1) ≤ r+1} C(r, m*(c+1)-1)
 * Range:     u8 n <= 35, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binom_row, compute_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_row.h"
#include "../utils/compute_T.h"

/* Main sequence function required by the build system.  */
seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t lo = 0, hi = n;      /* upper bound hi cannot exceed n */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t row = lo;
    idx_t start = row * (row + 1) / 2;
    idx_t col = n - start;

    return compute_T(row, col);
}
