/*
 * A023961  First digit after decimal point of square root of n.
 * https://oeis.org/A023961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = floor(sqrt(100*n)) - 10 * floor(sqrt(n))
 * Range:     u8 none, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9643 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to the first digit after the decimal point of sqrt(n). */

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
    seq_t n_seq   = (seq_t)n;                /* n as seq_t */
    seq_t sqrt_n  = floor_sqrt(n_seq);       /* floor(sqrt(n))          */
    seq_t sqrt_n100 = floor_sqrt((seq_t)100 * n_seq); /* floor(sqrt(100*n)) */
    seq_t digit = sqrt_n100 - (seq_t)10 * sqrt_n;     /* 0..9 */

    return digit;
}
