/*
 * A024108  a(n) = 9^n-n^7.
 * https://oeis.org/A024108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9^n - n^7
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 9, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Helpers:   big_pow, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Convert the index to the generic sequence type */
    seq_t sn = (seq_t)n;

    /* 9^n */
    seq_t pow9 = big_pow((seq_t)9, sn);

    /* n^7 */
    seq_t pow7 = pow_seq(sn, 7);

    /* Difference */
    return pow9 - pow7;
}
