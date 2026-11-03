/*
 * A190337  a(n) = n + [n*r/t] + [n*s/t]; r=2, s=sqrt(2), t=1/s.
 * https://oeis.org/A190337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n + floor(n*sqrt(8))
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9874 (2026-11-01)
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
    /* Convert the index to seq_t for all arithmetic. */
    seq_t ns = (seq_t)n;

    /* Compute n^2; used to avoid floating point. */
    seq_t n_sq = ns * ns;

    /* Compute 8 * n^2; this equals (n*sqrt(8))^2. */
    seq_t eight_n_sq = n_sq * 8;

    /* Floor of the square root gives floor(n*sqrt(8)). */
    seq_t f = floor_sqrt(eight_n_sq);

    /* a(n) = 3*n + floor(n*sqrt(8)). */
    return ns * 3 + f;
}
