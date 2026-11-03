/*
 * A033715  Number of integer solutions (x, y) to the equation x^2 + 2y^2 = n.
 * https://oeis.org/A033715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer solutions (x, y) to x^2 + 2y^2 = n
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 120 terms through n = 9678 (2026-11-01)
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

seq_t a(idx_t n) {
    seq_t count = 0;
    for (idx_t y = 0; ; ++y) {
        seq_t rhs = (seq_t)2 * (seq_t)y * (seq_t)y;
        if (rhs > (seq_t)n) break;          /* 2y^2 > n  → further y impossible */

        seq_t r = (seq_t)n - rhs;           /* remaining part for x^2  */

        seq_t sq = floor_sqrt(r);           /* floor(sqrt(r))   */
        if (sq * sq == r) {                 /* r is a perfect square */
            seq_t pair_cnt = (sq == 0) ? 1 : 2;   /* x = ±sqrt(r) */
            if (y == 0)
                count += pair_cnt;          /* only y = 0 counted once */
            else
                count += pair_cnt * 2;      /* both +y and –y */
        }
    }
    return count;
}
