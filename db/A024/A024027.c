/*
 * A024027  a(n) = 3^n - n^4.
 * https://oeis.org/A024027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^n - n^4
 * Range:     i8 n <= 3, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* Compute 3^n */
    seq_t pow3 = big_pow((seq_t)3, n_seq);

    /* Compute n^4 (multiplication is safe for the range of n used) */
    seq_t n4 = n_seq * n_seq * n_seq * n_seq;

    /* Return the exact value in the appropriate type */
    return pow3 - n4;
}
