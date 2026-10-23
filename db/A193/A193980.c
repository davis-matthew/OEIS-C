/*
 * A193980  Mirror of the triangle A193979.
 * https://oeis.org/A193980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((k^2 + k + 2)/2) * r + (-k^3 + 3k^2 - 2k + 6)/6 * where
 *            r = floor((sqrt(8*n + 1) - 1) / 2) and k = r - (n - r*(r+1)/2)
 * Range:     u8 n <= 44, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   w_triangle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/w_triangle.h"

/* Integer square root for 64‑bit values (floor). */
static inline idx_t
isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    idx_t r = (isqrt_u64(8 * n + 1) - 1) / 2;          /* row index */
    idx_t start = r * (r + 1) / 2;                     /* index of first term of row r */
    idx_t local = n - start;                           /* position within the row */
    idx_t k = r - local;                               /* mirrored column index */
    return w_triangle(r, k);
}
