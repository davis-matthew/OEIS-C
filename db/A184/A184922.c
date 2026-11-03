/*
 * A184922  a(n) = n + [rn/t] + [sn/t] + [un/t], where []=floor and r=2^(1/2),
 *          s=r+1, t=r+2, u=r+3.
 * https://oeis.org/A184922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(2+sqrt(2))) - 1
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
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

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          // n^2
    seq_t two_sq = sq * 2;       // 2*n^2
    seq_t s = floor_sqrt(two_sq);    // floor(sqrt(2*n^2)) = floor(n*sqrt2)
    seq_t res = nn * 2 - 1 + s; // 2n - 1 + floor(n*sqrt2)
    return res;
}
