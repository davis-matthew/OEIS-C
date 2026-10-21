/*
 * A210370  Number of 2 X 2 matrices with all elements in {0,1,...,n} and odd
 *          determinant.
 * https://oeis.org/A210370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * ((n+1)^2 – ceil(n/2)^2) * ceil(n/2)^2
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 326, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* ceil(n/2) */
    idx_t k = (n + 1) >> 1;

    /* (n+1)^2 and ceil(n/2)^2 as seq_t values */
    seq_t n1sq = seq_pow((seq_t)(n + 1), 2);
    seq_t ksq  = seq_pow((seq_t)k, 2);

    /* difference and product */
    seq_t diff = n1sq - ksq;
    seq_t prod = seq_mul(diff, ksq);
    prod = seq_mul(prod, 2);

    return prod;
}
