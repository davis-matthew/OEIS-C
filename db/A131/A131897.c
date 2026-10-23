/*
 * A131897  A130321 + A131821 - A000012.
 * https://oeis.org/A131897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the row index such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.
 *            Let c be the column index within that row, n = r*(r+1)/2 + c (0
 *            ≤ c ≤ r). Then the triangle entry is a(r,0) = 2^r + r (left
 *            column) a(r,r) = r + 1 (diagonal) otherwise a(r,c) = 2^(r-c) The
 *            sequence a(n) is the flattened triangle read...
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"

/* External types defined by the build system */
extern idx_t _floor_isqrt_u64(idx_t n);   /* unused but kept for reference */

seq_t a(idx_t n)
{
    /* Find row r: r = floor((sqrt(8*n+1)-1)/2) */
    idx_t r = (idx_t)((sqrt(8.0 * n + 1.0) - 1.0) / 2.0);

    /* Correct possible rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r * (r + 1) / 2 > n) r--;

    idx_t start = r * (r + 1) / 2;          /* first index of this row   */
    idx_t c     = n - start;                /* column index in the row   */

    if (c == 0)
    {
        /* Left column: 2^r + r */
        return pow2(r) + (seq_t)r;
    }
    else if (c == r)
    {
        /* Diagonal: r + 1 */
        return (seq_t)(r + 1);
    }
    else
    {
        /* Off‑diagonal: 2^(r-c) */
        return pow2(r - c);
    }
}
