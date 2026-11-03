/*
 * A224985  Floor of the Euclidean distance of a point on the (1, 1, 1; 2, 2,
 *          2) 3D walk.
 * https://oeis.org/A224985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 34, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = ⌊√(X² + Y² + Z²)⌋, with
 *     X = T(c) + L·[n mod 3 ≥ 1]
 *     Y = T(c) + L·[n mod 3 ≥ 2]
 *     Z = T(c)
 * where c = ⌊n/3⌋, L = c+1, T(c) = c(c+1)/2.
 * This matches the Euclidean distance after n steps of the walk.
 */
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t c = n / 3;
    idx_t r = n % 3;
    seq_t tri = (seq_t)c * (c + 1) / 2;
    seq_t L    = (seq_t)(c + 1);
    seq_t x = tri, y = tri, z = tri;
    if (r >= 1) x += L;
    if (r >= 2) y += L;
    seq_t sum = x * x + y * y + z * z;
    return floor_sqrt(sum);
}
