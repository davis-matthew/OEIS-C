/*
 * A135962  a(n) = binomial(floor(n*(sqrt(5)+1)/2), n) for n>=0.
 * https://oeis.org/A135962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( floor(n*(sqrt(5)+1)/2) , n )
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   binomial, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
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
    /* Compute m = floor(n*φ) using integer arithmetic.
       m = floor((n + floor( n*sqrt(5) )) / 2) ,
       and floor(n*sqrt(5)) == floor_sqrt(5*n^2). */
    seq_t n_seq = (seq_t)n;
    seq_t nsq = n_seq * n_seq;          /* n^2          */
    seq_t five_nsq = nsq * (seq_t)5;    /* 5*n^2        */
    seq_t sq = floor_sqrt(five_nsq);    /* floor(n*sqrt5)
                                           (because sqrt(5)*n = sqrt(5*n^2)) */
    seq_t sum = n_seq + sq;             /* n + floor(n*sqrt5) */
    seq_t m_seq = sum / (seq_t)2;       /* floor((n + floor(n*sqrt5))/2) */
    idx_t m = (idx_t)m_seq;             /* convert to idx_t for binomial            */
    return binomial(m, n);
}
