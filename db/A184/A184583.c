/*
 * A184583  floor[(n-1/5)(1+r)], where r=(1+sqrt(5))/2; complement of A184582.
 * https://oeis.org/A184583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n - 1/5) * (1 + r)) where r = (1+sqrt(5))/2
 * Range:     u8 none, u16 n <= 11, u32 n <= 120, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;            // offset starts at 1

    seq_t B = 5 * n - 1;             // m = 5n - 1
    seq_t sqrt_term = floor_sqrt((seq_t)5 * B * B);  // floor(B * sqrt5)
    seq_t numerator = (seq_t)3 * B + sqrt_term;     // 3B + floor(B*sqrt5)
    return numerator / 10;          // floor((3B + B*sqrt5)/10)
}
