/*
 * A195172  a(n) = 4*n - floor(2*n*r), where r=(1+sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A195172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - floor(n*sqrt5) (equivalent to a(n) = 4*n -
 *            floor(2*n*phi))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9200 (2026-11-01)
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
    /* compute floor(n*sqrt5) as floor(sqrt(5*n*n)) */
    seq_t nsq   = (seq_t)n * (seq_t)n;   /* n^2 in seq_t */
    seq_t five_nsq = nsq * 5;            /* 5*n^2 in seq_t */
    seq_t fsqrt  = floor_sqrt(five_nsq); /* floor(sqrt(5*n^2)) = floor(n*sqrt5) */
    seq_t term   = (seq_t)3 * (seq_t)n;  /* 3*n in seq_t */
    return term - fsqrt;                 /* result fits in seq_t */
}
