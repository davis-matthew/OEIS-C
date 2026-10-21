/*
 * A230211  Trapezoid of dot products of row 8 (signs alternating) with
 *          sequential 9-tuples read by rows in Pascal's triangle A007318:
 *          T(n,k) is the linear combination of the 9-tuples (C(8,0), -C(8,1),
 *          ..., -C(8,7), C(8,8)) and (C(n-1,k-8), C(n-1,k-7), ..., C(n-1,k)),
 *          n >= 1, 0 <= k <= n+7.
 * https://oeis.org/A230211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabf
 * Formula:   a(n) = Sum_{j=0..8} (-1)^j * C(8,j) * C(r-1, k-j) where n is the
 *            flat index into the trapezoid, r is the row number (r≥1, row r
 *            has r+8 terms), and k is 0‑based column index within the row (0
 *            ≤ k ≤ r+7)
 * Range:     i8 n <= 125, i16 n <= 305, i32 n <= 892, i64 n <= 1675,
 *            i128 n <= 1675, big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mul_small.h"

/* Helper: binomial that returns 0 if k > n or k < 0. */
static inline seq_t safe_binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    return binomial(n, k);
}

/* Return the n‑th term of sequence A230211 (offset 1). */
seq_t a(idx_t n)
{
    /* Find the row r such that cumulative(r‑1) < n ≤ cumulative(r).
       Cumulative terms up to row r: C(r) = r(r+17)/2. */
    idx_t lo = 1, hi = 1;
    while ((hi * (hi + 17ULL)) / 2ULL < n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((mid * (mid + 17ULL)) / 2ULL < n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t r = lo;                /* row number (1‑based) */
    idx_t prev = (r - 1) * (r + 16ULL) / 2ULL;   /* terms before this row */
    idx_t offset = n - 1 - prev;                /* zero‑based position in row */
    idx_t k = offset;                               /* column index in row */

    /* Coefficients C(8,j) with alternating signs:
       +1, -8, +28, -56, +70, -56, +28, -8, +1 */
    static const int coeff[9] = {1, -8, 28, -56, 70, -56, 28, -8, 1};

    seq_t sum = 0;
    for (idx_t j = 0; j < 9; ++j) {
        if (k < j) continue;                    /* k-j < 0 → binomial = 0 */
        seq_t bin = safe_binomial(r - 1, k - j);
        if (!bin) continue;                     /* binomial is small; skip if zero */
        seq_t term = mul_small(bin, (unsigned int)(coeff[j] > 0 ? coeff[j] : -coeff[j]));
        if (coeff[j] < 0) term = -term;
        sum += term;
    }
    return sum;
}
