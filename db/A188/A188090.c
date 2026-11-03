/*
 * A188090  [nr+kr]-[nr]-[kr], where r=sqrt(3), k=5, [ ]=floor.
 * https://oeis.org/A188090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+5)*sqrt(3)) - floor(n*sqrt(3)) - 8
 *            (equivalently, a(n)=1 iff the fractional part of n*sqrt(3) ≥
 *            1‑frac(5*sqrt(3)), else 0)
 * Range:     u8 none, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 156 terms through n = 9749 (2026-11-01)
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
    /* Compute floor(n*sqrt(3)) as floor_sqrt(3*n*n)  */
    seq_t nn = (seq_t)n;
    seq_t sq_n = floor_sqrt((seq_t)3 * nn * nn);

    /* Compute floor((n+5)*sqrt(3)) as floor_sqrt(3*(n+5)^2) */
    nn += (seq_t)5;
    seq_t sq_n5 = floor_sqrt((seq_t)3 * nn * nn);

    return sq_n5 - sq_n - (seq_t)8;
}
