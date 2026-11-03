/*
 * A208326  n + [nr/t] + [ns/t], where []=floor, r=5, s=(1+sqrt(5))/2, t=1/s.
 * https://oeis.org/A208326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n·5φ⌋ + ⌊n·φ²⌋ where φ = (1+√5)/2, φ² = φ+1. The
 *            computation avoids floating point by using: ⌊n·(p+q√5)/2⌋ =
 *            ⌊(p·n + ⌊q·n√5⌋)/2⌋ and evaluating ⌊k√5⌋ as ⌊√(5k²)⌋. The helper
 *            floor_sqrt(seq_t) is provided in the utils directory. It returns
 *            ⌊√x⌋ for any seq_t x
 * Range:     u8 none, u16 n <= 11, u32 n <= 2646, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9958 (2026-11-01)
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
    seq_t nn = (seq_t)n;

    /* First term: ⌊n·5φ⌋  = ⌊(5·n + ⌊5n√5⌋)/2⌋ */
    seq_t k1   = nn * 5;                     /* 5·n */
    seq_t sq1  = k1 * k1 * 5;                /* 5·(5n)² = 5k1²  */
    seq_t fp1  = floor_sqrt(sq1);            /* ⌊5n√5⌋  */
    seq_t num1 = 5 * nn + fp1;               /* 5·n + ⌊5n√5⌋ */
    seq_t f1   = num1 / 2;                   /* ⌊(5·n + ⌊5n√5⌋)/2⌋ */

    /* Second term: ⌊n·φ²⌋ = ⌊(3·n + ⌊n√5⌋)/2⌋ */
    seq_t k2   = nn;                         /* n */
    seq_t sq2  = k2 * k2 * 5;                /* 5·n²   */
    seq_t fp2  = floor_sqrt(sq2);            /* ⌊n√5⌋  */
    seq_t num2 = 3 * nn + fp2;               /* 3·n + ⌊n√5⌋ */
    seq_t f2   = num2 / 2;                   /* ⌊(3·n + ⌊n√5⌋)/2⌋ */

    return nn + f1 + f2;
}
