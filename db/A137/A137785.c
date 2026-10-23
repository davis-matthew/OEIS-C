/*
 * A137785  Triangular sequence of coefficients of the expansion of p(x,t) =
 *          exp(x*t)*(1 + t^2)^2/(t*(1 - t^2)).
 * https://oeis.org/A137785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, uned
 * Formula:   a(n) = coefficient of x^(pos-1) in the polynomial * g_{row-1}(x)
 *            = (row)! * Σ_{m=0}^{row} r_{(row-1)-m} * x^m / m!, * where
 *            r_{-1}=1, r_1=3, r_j=4 for odd j≥3, otherwise r_j=0. * The
 *            sequence a(n) is obtained by flattening the rows of this *
 *            triangular array, row 1 having 2 elements, row 2 having...
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   triangular_cumsum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/triangular_cumsum.h"

/* Returns the nth term of the sequence (1‑based index). */
seq_t a(idx_t n) {
    /* Find the row r such that T(r-1) < n ≤ T(r). */
    idx_t low = 1, high = 1;
    while (triangular_cumsum(high) < n) high <<= 1;          /* exponential search */
    while (low < high) {
        idx_t mid = (low + high) / 2;
        if (triangular_cumsum(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                 /* row number (1‑based) */
    idx_t prev_sum = triangular_cumsum(r - 1);
    idx_t pos = n - prev_sum;      /* position within the row (1‑based) */
    idx_t k = pos - 1;             /* column index, 0‑based exponent of x */
    idx_t n0 = r - 1;              /* row index used in g_n (zero‑based) */

    /* Compute j = n0 - k.  Use signed arithmetic to allow -1. */
    long long j = (long long)n0 - (long long)k;
    if (j < -1 || ((j & 1LL) == 0))           /* j even or j < -1 => coefficient 0 */
        return (seq_t)0;

    /* Determine the small factor r_j. */
    unsigned char factor;                     /* 1, 3, or 4 */
    if (j == -1)
        factor = 1;
    else if (j == 1)
        factor = 3;
    else
        factor = 4;                           /* j is odd and ≥ 3 */

    /* Compute (n0+1)! / k! as product from t = k+1 to n0+1. */
    seq_t res = 1;
    for (idx_t t = k + 1; t <= n0 + 1; ++t)
        res *= (seq_t)t;

    res *= (seq_t)factor;
    return res;
}
