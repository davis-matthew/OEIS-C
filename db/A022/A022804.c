/*
 * A022804  a(n) = B(n) + c(n) where B(n) is Beatty sequence [ n*sqrt(2) ] and
 *          c is the complement of B.
 * https://oeis.org/A022804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n + floor(n*sqrt(2)))
 * Range:     u8 n <= 5, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9452 (2026-11-01)
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
    if (n == 0) return (seq_t)0;          /* offset is 1 */

    seq_t sn = (seq_t)n;                  /* convert index to seq_t */
    seq_t sq = sn * sn;                   /* n^2   */
    seq_t dbl_sq = sq + sq;               /* 2 * n^2 = (n*sqrt(2))^2   */
    seq_t fl_sqrt = floor_sqrt(dbl_sq);   /* floor(n*sqrt(2))   */
    seq_t res = (sn + fl_sqrt) * (seq_t)2;/* 2*(n + floor(n*sqrt(2))) */
    return res;
}
