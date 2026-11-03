/*
 * A194169  a(n) = [Sum_{k=1..n} (k*r)], where [ ]=floor, ( )=fractional part,
 *          and r=sqrt(5).
 * https://oeis.org/A194169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊√5 · n(n+1)/2⌋ – Σ_{k=1..n} ⌊√5 · k⌋
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 73, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 89 terms through n = 4472 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (For r = √5, a(n) equals the integer part of the sum of the fractional parts of k r.) */
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
    /* N = n(n+1)/2, computed in seq_t to avoid overflow of the intermediate product */
    seq_t N = (seq_t)n * ((seq_t)n + 1) / 2;

    /* T = ⌊√5 N⌋  =  ⌊√(5 N²)⌋  */
    seq_t T = floor_sqrt((seq_t)5 * N * N);

    /* Sum of floor(k √5) for k = 1..n */
    seq_t sumFloor = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t val = floor_sqrt((seq_t)5 * kk * kk);
        sumFloor += val;
    }

    return T - sumFloor;
}
