/*
 * A024057  a(n) = 5^n - n^8.
 * https://oeis.org/A024057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert n to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* Compute 5^n using the big‑integer utility */
    seq_t pow5 = big_pow((seq_t)5, n);

    /* Compute n^8 by repeated multiplication */
    seq_t pow8 = 1;
    for (idx_t i = 0; i < 8; ++i)
        pow8 *= n_seq;

    /* Return the difference */
    return pow5 - pow8;
}
