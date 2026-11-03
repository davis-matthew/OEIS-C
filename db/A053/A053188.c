/*
 * A053188  Distance from n to nearest square.
 * https://oeis.org/A053188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = |floor(sqrt(n)+1/2)^2 - n|
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9820 (2026-11-01)
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
    seq_t N = (seq_t)n;
    seq_t r = floor_sqrt(N);
    seq_t r2 = r * r;
    seq_t lowDiff = N - r2;
    seq_t r1 = r + (seq_t)1;
    seq_t r12 = r1 * r1;
    seq_t highDiff = r12 - N;
    return (lowDiff <= highDiff) ? lowDiff : highDiff;
}
