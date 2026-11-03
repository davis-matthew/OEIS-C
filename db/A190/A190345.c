/*
 * A190345  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(5/2), t=sqrt(2/5).
 * https://oeis.org/A190345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( n*sqrt(2/5) ) + floor( 2*n/5 )
 * Range:     u8 n <= 11, u16 n <= 558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9829 (2026-11-01)
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

/* Computes OEIS A190345: g(n) = n + [n*sqrt(2/5)] + [2*n/5]  */
seq_t a(idx_t n)
{
    /* cast the index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* integer part of 2*n/5 */
    seq_t term2 = (n_seq * 2) / 5;

    /* compute floor( n*sqrt(2/5) ) as floor( sqrt( (2*n*n)/5 ) ) */
    seq_t temp   = (n_seq * n_seq) * (seq_t)2;   /* 2*n*n   */
    seq_t numer  = temp / 5;                     /* (2*n*n)/5  (floor division)   */
    seq_t term3  = floor_sqrt(numer);            /* floor sqrt of numerator */

    /* result */
    return n_seq + term2 + term3;
}
