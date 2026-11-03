/*
 * A172323  a(n) = floor(n*(sqrt(5)+sqrt(2))).
 * https://oeis.org/A172323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor(n*(sqrt(5)+sqrt(2)))
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 449, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 909 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses exact integer arithmetic and the floor_sqrt
   helper. It avoids floating‐point arithmetic while remaining
   accurate for all n that fit into the resulting seq_t */ 

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
    /* n^2 as seq_t */
    seq_t nsq = (seq_t)n * n;

    /* floor(n*sqrt(5)) and floor(n*sqrt(2)) */
    seq_t a_floor = floor_sqrt((seq_t)5 * nsq);
    seq_t b_floor = floor_sqrt((seq_t)2 * nsq);

    /* Sum of floors */
    seq_t sum_floor = a_floor + b_floor;

    /* Determine whether the fractional parts sum to >= 1.
       Carry = 1 if floor(n*α) > a_floor + b_floor. */
    seq_t s = sum_floor + 1;           /* A + B + 1          */
    seq_t s_sq = s * s;                /* (A + B + 1)^2     */
    seq_t seg_coeff = (seq_t)7 * nsq;  /* 7 n^2              */
    seq_t diff = (s_sq >= seg_coeff) ? (s_sq - seg_coeff)
                                     : (seg_coeff - s_sq);  /* |(A+B+1)^2 - 7n^2| */

    seq_t nsq_sq = nsq * nsq;          /* n^4                         */
    seq_t lhs = (seq_t)40 * nsq_sq;    /* 40 n^4                      */
    seq_t diff_sq = diff * diff;       /* (|(A+B+1)^2 - 7n^2|)^2       */

    seq_t carry = (lhs >= diff_sq) ? 1 : 0;

    return sum_floor + carry;
}
