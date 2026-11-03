/*
 * A195122  a(n) = 2*n - floor(n*r/2), where r=(1+sqrt(5))/2.
 * https://oeis.org/A195122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n - floor(n*r/2), where r = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8469 (2026-11-01)
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
    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t val = (seq_t)5 * nn * nn;    /* 5 · n²  */
    seq_t sqrt5 = floor_sqrt(val);     /* floor(n·sqrt(5)) */
    seq_t m = (nn + sqrt5) / 4;        /* floor(n·φ/2) */
    return 2 * nn - m;                 /* a(n) = 2n – m */
}
