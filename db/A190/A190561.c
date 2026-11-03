/*
 * A190561  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),4,3) and
 *          []=floor.
 * https://oeis.org/A190561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4*n+3)*sqrt(2)) - 4*floor(n*sqrt(2)) - 4
 * Range:     u8 none, u16 n <= 21, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 154 terms through n = 964 (2026-11-01)
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
    /* Compute floor((4*n+3)*sqrt(2)) */
    idx_t t = (idx_t)4 * n + 3;
    /* 2*t*t may fit in 64‑bit for typical inputs (tests up to 10k) */
    idx_t tt = (idx_t)2 * t * t;
    seq_t first = floor_sqrt((seq_t)tt);

    /* Compute floor(n*sqrt(2)) */
    idx_t nn = (idx_t)2 * n * n;
    seq_t second = floor_sqrt((seq_t)nn);

    /* Final a(n) value */
    return first - (seq_t)4 * second - (seq_t)4;
}
