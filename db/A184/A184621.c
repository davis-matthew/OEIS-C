/*
 * A184621  a(n) = floor((n-h)*s+h), where s=2+sqrt(2) and h=1/4; complement
 *          of A184620.
 * https://oeis.org/A184621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1/4)*(2+sqrt(2))+1/4)
 * Range:     u8 n <= 2, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 8496 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements A184621 using only integer arithmetic. */
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
    seq_t n_seq = (seq_t)n;

    /* t = floor(n*sqrt(2))  via floor_sqrt(2*n^2) */
    seq_t tn   = n_seq * n_seq;
    seq_t two_n2 = (seq_t)2 * tn;
    seq_t t = floor_sqrt(two_n2);

    /* p = 4*n - 1 */
    seq_t p = (seq_t)4 * n_seq - 1;

    /* Check whether fractional part of n*sqrt(2) is < (1+sqrt(2))/4 */
    seq_t left  = (seq_t)2 * p * p;                     /* 2*p^2 */
    seq_t four_t_plus1 = (seq_t)4 * t + 1;              /* 4*t + 1 */
    seq_t right = four_t_plus1 * four_t_plus1;          /* (4*t+1)^2 */

    seq_t result = (seq_t)2 * n_seq + t;
    if (left < right) {
        /* fractional part < (1+sqrt(2))/4, so subtract 1 */
        result -= 1;
    }
    return result;
}
