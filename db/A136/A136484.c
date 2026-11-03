/*
 * A136484  Number of unit square lattice cells inside quadrant of origin
 *          centered circle of diameter 2n+1.
 * https://oeis.org/A136484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(sqrt((n+1/2)^2 - k^2))
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 834 (2026-11-01)
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
    seq_t total = 0;
    seq_t two_n_plus_one = (seq_t)n * 2 + 1;
    seq_t r_sq = two_n_plus_one * two_n_plus_one;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t k_sq = (seq_t)k * k;
        seq_t val_sq = r_sq - (seq_t)4 * k_sq;
        seq_t s = floor_sqrt(val_sq);
        total += s >> 1;                 /* floor_sqrt(val_sq)/2 */
    }
    return total;
}
