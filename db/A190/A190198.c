/*
 * A190198  a(n) = [n*u+n*v]-[n*u]-[n*v], where u=sqrt(3), v=1/u, and
 *          []=floor.
 * https://oeis.org/A190198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*u+n*v]-[n*u]-[n*v]
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 4516, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 8370 (2026-11-01)
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

/* seq_t a(idx_t n)
   Computes a(n) = floor(n*sqrt(3)+n/sqrt(3)) - floor(n*sqrt(3)) - floor(n/sqrt(3))
   using only integer arithmetic. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* not part of the OEIS offset, but safe */
    seq_t nn = (seq_t)n;
    /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    seq_t s = floor_sqrt((seq_t)3 * nn * nn);
    /* floor(n*v) = floor(n/sqrt(3)) = floor(s/3) */
    seq_t r = s / 3;
    /* floor((4n*sqrt(3))/3) = floor(sqrt(48*n^2)/3) */
    seq_t t = floor_sqrt((seq_t)48 * nn * nn) / 3;
    return t - s - r;
}
