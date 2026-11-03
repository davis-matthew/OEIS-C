/*
 * A135034  Positive integers n repeated 2n-1 times, with a leading a(0) = 0.
 *          Also: ceiling of square root of n.
 * https://oeis.org/A135034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(sqrt(n))
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 92 terms through n = 981 (2026-11-01)
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
    if (n == 0) return (seq_t)0;
    seq_t m = floor_sqrt((seq_t)n);           /* floor(sqrt(n)) */
    uint64_t mm = (uint64_t)m * (uint64_t)m;   /* (floor(sqrt(n)))², safe in 64‑bit */
    if (mm == n) return m;                    /* perfect square */
    return m + 1;                             /* otherwise ceil */
}
