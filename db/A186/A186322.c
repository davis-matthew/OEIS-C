/*
 * A186322  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the squares and heptagonal
 *          numbers. Complement of A186323.
 * https://oeis.org/A186322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((3 + sqrt(9 + 40*n^2))/10)
 * Range:     u8 n <= 1, u16 n <= 20, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
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
    seq_t sn = (seq_t)n;                 /* index as seq_t */
    seq_t sq = sn * sn;                  /* n^2 */
    seq_t N  = sq * (seq_t)40 + (seq_t)9;/* 40*n^2 + 9 */
    seq_t sqrt_val = floor_sqrt(N);      /* floor(sqrt(9 + 40*n^2)) */
    seq_t j = (sqrt_val + (seq_t)3) / (seq_t)10;
    return sn + j;
}
