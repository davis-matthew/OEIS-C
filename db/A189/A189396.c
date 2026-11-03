/*
 * A189396  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=1/sqrt(2), t=sqrt(3).
 * https://oeis.org/A189396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n*sqrt(6))
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 989 (2026-11-01)
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
    /* Convert index to seq_t for the arithmetic */
    seq_t N = (seq_t)n;
    seq_t result = N;                 /* the 'n' term */
    
    /* floor(n*sqrt(2))  == floor_sqrt(2*n*n) */
    seq_t x2 = N * N;                  /* n*n */
    x2 = x2 * (seq_t)2;                /* 2*n*n */
    seq_t f2 = floor_sqrt(x2);
    result = result + f2;
    
    /* floor(n*sqrt(6))  == floor_sqrt(6*n*n) */
    seq_t x6 = N * N;                  /* n*n */
    x6 = x6 * (seq_t)6;                /* 6*n*n */
    seq_t f6 = floor_sqrt(x6);
    result = result + f6;
    
    return result;
}
