/*
 * A054110  Triangular array T(n,k): in Pascal's triangle, draw vertical lines
 *          through binomial(n-1,k-1) (if present) and binomial(n-1,k) (if
 *          present); then T(n,k) is the sum of binomial(i,j) that lie on or
 *          between the lines and not below binomial(n,k).
 * https://oeis.org/A054110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k), where T is the triangular array defined by
 *            summing binomial coefficients inside vertical bands in Pascal's
 *            triangle. The sequence is obtained by flattening the array row
 *            by row
 * Range:     u8 n <= 39, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, ceil_div2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/ceil_div2.h"

/* Helper: floor division by 2 for signed integers. */
static inline int64_t floor_div2(int64_t a) {
    return (a >= 0) ? a / 2 : (a - 1) / 2;
}

/* Return the nth element of OEIS sequence A054110. */
seq_t
a(idx_t n)
{
    /* Empty triangle case */
    if (n == 0) return (seq_t)1;

    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* Special handling for the first row (already returned above). */

    /* Compute the x–coordinate bounds for the vertical band. */
    int64_t rminus1 = (int64_t)r - 1;            /* r-1 may be -1 when r=0 */
    int64_t leftVal, rightVal;

    /* Left bound (exists if k>0). */
    if (k > 0) {
        leftVal = 2 * (int64_t)(k - 1) - rminus1;
    } else {
        leftVal = 2 * (int64_t)k - rminus1;    /* band degenerates to one line */
    }

    /* Right bound (exists if k < r). */
    if (k < r) {
        rightVal = 2 * (int64_t)k - rminus1;
    } else {
        rightVal = 2 * (int64_t)(k - 1) - rminus1;
    }

    /* Sum all binomial coefficients within the band. */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i <= r; ++i) {
        int64_t a1 = (int64_t)i + leftVal;
        int64_t a2 = (int64_t)i + rightVal;

        int64_t j_low  = ceil_div2(a1);
        int64_t j_high = floor_div2(a2);

        if (j_low < 0) j_low = 0;
        if (j_high > (int64_t)i) j_high = i;

        if (j_low <= j_high) {
            for (idx_t j = (idx_t)j_low; j <= (idx_t)j_high; ++j) {
                result += binomial(i, j);
            }
        }
    }

    return result;
}
