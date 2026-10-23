/*
 * A135962  a(n) = binomial(floor(n*(sqrt(5)+1)/2), n) for n>=0.
 * https://oeis.org/A135962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial( floor(n*(sqrt(5)+1)/2) , n )
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

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
