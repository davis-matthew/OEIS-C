/*
 * A189377  a(n) = n + floor(ns/r) + floor(nt/r) with r=2, s=(-1+sqrt(5))/2,
 *          t=(1+sqrt(5))/2.
 * https://oeis.org/A189377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((floor(sqrt(5*n*n)) - n)/4) +
 *            floor((floor(sqrt(5*n*n)) + n)/4)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-01)
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
    seq_t nn = n;
    seq_t m  = (seq_t)5 * nn * nn;          /* 5·n² */
    seq_t s  = floor_sqrt(m);               /* floor(√(5·n²)) = floor(n·√5) */
    seq_t A  = (s - nn) / (seq_t)4;         /* floor((s - n)/4)  */
    seq_t B  = (s + nn) / (seq_t)4;         /* floor((s + n)/4)  */
    return nn + A + B;                      /* a(n) = n + A + B */
}
