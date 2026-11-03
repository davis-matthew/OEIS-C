/*
 * A033796  Product t2(q^d); d | 37, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (x,y) ∈ ℕ² | x,y odd, x² + 37·y² = 8·n + 38 }
 * Range:     u8 n <= 4, u16 n <= 1810, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 8817 (2026-11-01)
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
    /* Compute N = 8*n + 38 */
    seq_t N = ((seq_t)n) * 8 + 38;
    seq_t count = 0;

    /* Iterate over odd y values. */
    for (seq_t y = 1; ; y += 2) {
        seq_t sub = (seq_t)37 * y * y;   /* 37·y² */
        if (sub >= N)                    /* RHS would be ≤ 0 */
            break;

        seq_t rhs = N - sub;             /* N - 37·y² > 0 */
        seq_t r   = floor_sqrt(rhs);     /* floor √rhs */

        /* Check that rhs is a perfect square and the square root is odd. */
        if (r * r == rhs && (r & 1))
            ++count;
    }

    return count;
}
