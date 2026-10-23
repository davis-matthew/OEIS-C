/*
 * A190345  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(5/2), t=sqrt(2/5).
 * https://oeis.org/A190345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( n*sqrt(2/5) ) + floor( 2*n/5 )
 * Range:     u8 n <= 11, u16 n <= 558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

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
