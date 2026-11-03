/*
 * A215781  a(n) = ceiling(n*(sqrt(3)-1)).
 * https://oeis.org/A215781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*(sqrt(3)-1))
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 499, u64 n <= 499,
 *            u128 n <= 499, big any n
 * Validated: standard, pass, 74 terms through n = 425 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the identity ceil(n*(sqrt(3)-1)) = floor(n*sqrt(3)) - n + 1 for n>0 */
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

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute floor(n*sqrt(3)) as floor(sqrt(3*n*n)) */
    seq_t sq = (seq_t)3 * (seq_t)n * (seq_t)n;
    seq_t floor_root = floor_sqrt(sq);

    return floor_root - (seq_t)n + 1;
}
