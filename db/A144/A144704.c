/*
 * A144704  a(n) = 4^n*(1-2*n).
 * https://oeis.org/A144704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4^n*(1-2*n)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 13, i64 n <= 28, i128 n <= 60,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* 1 - 2*n computed in seq_t to avoid overflow on intermediate */
    seq_t two_n  = ((seq_t)2) * (seq_t)n;
    seq_t pow4   = pow_seq((seq_t)4, (int)n);
    seq_t result = pow4 * ( ((seq_t)1) - two_n );
    return result;
}
