/*
 * A020965  a(n) = Sum_{k >= 1} floor(n*sqrt(2)^(1-k)).
 * https://oeis.org/A020965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k>=1} floor( n * sqrt(2)^(1-k) )
 * Range:     u8 n <= 11, u16 n <= 132, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 972 (2026-11-01)
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

/* Compute a(n) – seq_t is an unsigned integer type (possibly arbitrary precision) */
seq_t a(idx_t n)
{
    seq_t seq_n = (seq_t)n;
    seq_t even = seq_n;                     /* floor(n / 2^j)  (j = 0 initially) */
    /* odd_start = floor(n / sqrt(2))  computed once */
    seq_t tmp   = seq_n * seq_n;            /* n^2          */
    seq_t half  = tmp / 2;                  /* floor(n^2 / 2) */
    seq_t odd   = floor_sqrt(half);         /* floor( n / sqrt(2) ) */

    seq_t sum = 0;
    while (even > 0 || odd > 0) {
        sum += even;
        if (odd > 0) sum += odd;
        even = even / 2;                    /* next even term: floor(n / 2^{j+1})  */
        odd  = odd  / 2;                    /* next odd term  : floor(n / (2^{j+1} sqrt(2))) */
    }
    return sum;
}
