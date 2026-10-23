/*
 * A183627  Number of (n+1) X 5 0..2 arrays with every 2 x 2 subblock summing
 *          to 4.
 * https://oeis.org/A183627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(60 + 5*2^(n+2) + 3^n)
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* 2^(n+2) : exponent fits in an int for all n <= 10000 */
    seq_t two_pow  = pow_seq((seq_t)2, (int)(n + 2));

    /* 3^n */
    seq_t three_pow = pow_seq((seq_t)3, (int)n);

    /* compute inner term 60 + 5*2^(n+2) + 3^n */
    seq_t inner = (seq_t)60
                + (seq_t)5 * two_pow
                + three_pow;

    /* final result */
    return inner * (seq_t)3;
}
