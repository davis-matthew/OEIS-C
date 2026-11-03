/*
 * A028625  Expansion of (theta_3(z)*theta_3(15z)+theta_2(z)*theta_2(15z)).
 * https://oeis.org/A028625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions (x,y) to x² + 15*y² = 4*n
 * Range:     u8 n <= 15, u16 n <= 4020, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 120 terms through n = 4963 (2026-11-01)
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
    seq_t count = 0;
    seq_t four_n = (seq_t)4 * (seq_t)n;

    for (idx_t y = 0; ; ++y)
    {
        seq_t y_sq   = (seq_t)y * (seq_t)y;
        seq_t prod   = (seq_t)15 * y_sq;
        if (prod > four_n) break;

        seq_t t    = four_n - prod;
        seq_t s    = floor_sqrt(t);
        if (s * s == t)
        {
            seq_t ways = (s == 0) ? (seq_t)1 : (seq_t)2;
            if (y == 0)
                count += ways;
            else
                count += ways * (seq_t)2;   /* ±y */
        }
    }
    return count;
}
