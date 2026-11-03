/*
 * A049740  a(n)=T(n,n), array T as in A049735.
 * https://oeis.org/A049740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer lattice points (x,y) with x^2 + y^2 ≤ 2
 *            n²
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
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
    /* r² = 2 n² */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t r2   = (seq_t)2 * n_sq;

    /* r = ⌊√r²⌋ */
    idx_t m = (idx_t)floor_sqrt(r2);

    seq_t total = 0;
    for (idx_t x = 0; x <= m; ++x) {
        seq_t rem = r2 - (seq_t)x * (seq_t)x;
        seq_t y_max = floor_sqrt(rem);
        seq_t count = (seq_t)2 * y_max + (seq_t)1;
        if (x == 0)
            total += count;
        else
            total += (seq_t)2 * count;
    }
    return total;
}
