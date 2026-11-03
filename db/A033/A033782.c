/*
 * A033782  Product t2(q^d); d | 23, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd solutions to x^2 + 23 y^2 = 8(n +
 *            3)
 * Range:     u8 n <= 7, u16 n <= 1914, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9949 (2026-11-01)
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
    seq_t target = 8 * ((seq_t)n + 3);
    seq_t limit = floor_sqrt(target / 23);

    for (idx_t y = 1; y <= (idx_t)limit; y += 2) {
        seq_t y_sq  = (seq_t)y * (seq_t)y;
        seq_t sub   = 23 * y_sq;
        if (sub > target) break;

        seq_t cur   = target - sub;
        seq_t x     = floor_sqrt(cur);

        if (x * x == cur && (x & 1) == 1) {
            count++;
        }
    }

    return count;
}
