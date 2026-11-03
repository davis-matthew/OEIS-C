/*
 * A080343  a(n) = round(sqrt(2*n)) - floor(sqrt(2*n)).
 * https://oeis.org/A080343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sqrt(2*n)) - floor(sqrt(2*n))
 * Range:     u8 n <= 31, u16 n <= 8076, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9993 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    seq_t N   = (seq_t)n;        /* Convert index to seq_t */
    seq_t twoN = N + N;          /* 2*n */
    seq_t m    = floor_sqrt(twoN);  /* floor(sqrt(2*n)) */
    seq_t r    = twoN - m * m;   /* remainder 2*n - m^2   */
    /* a(n)=1 iff the fractional part of sqrt(2*n) is ≥ 0.5,
       which is equivalent to r ≥ m+1 */
    return (r >= m + (seq_t)1) ? (seq_t)1 : (seq_t)0;
}
