/*
 * A131416  (A000012 * A002260) + (A002260 * A000012) - A000012.
 * https://oeis.org/A131416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) generated from the lower‐triangular matrix defined by
 *            (A000012 * A002260) + (A002260 * A000012) - A000012. With
 *            A002260_{i,j} = j for 1 ≤ j ≤ i, one obtains A_{i,j} =
 *            2*j*(i-j+1) + (i-j+1)*(i-j)/2 - 1. The sequence a(n) is the
 *            triangle read by rows (offset=1)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t sqrt_floor_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2 */
    idx_t t = sqrt_floor_u64((idx_t)8 * n + 1);
    idx_t r = (t - 1) / 2;
    if ((r * (r + 1)) / 2 < n)
        r++;

    /* First index of this row */
    idx_t start = ((r - 1) * r) / 2 + 1;
    /* Column position (0‑based) */
    idx_t col = n - start;
    idx_t j   = col + 1;            /* column index (1‑based) */
    idx_t d   = r - j;              /* difference i-j ≥ 0 */
    seq_t m   = (seq_t)(d + 1);

    /* A_{i,j} = 2*j*m + m*d/2 - 1 */
    seq_t value = (seq_t)2 * (seq_t)j * m + (m * (seq_t)d) / 2 - (seq_t)1;
    return value;
}
