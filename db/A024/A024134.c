/*
 * A024134  a(n) = 11^n - n^7.
 * https://oeis.org/A024134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11^n - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 18, i128 n <= 36,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute 11^n */
    seq_t pow_11 = big_pow((seq_t)11, n_seq);

    /* Compute n^7 */
    seq_t pow_n7 = big_pow(n_seq, (seq_t)7);

    /* Return a(n) = 11^n - n^7 */
    return pow_11 - pow_n7;
}
