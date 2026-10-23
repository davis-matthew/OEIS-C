/*
 * A024083  a(n) = 7^n - n^8.
 * https://oeis.org/A024083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7^n - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 11, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 7^n  */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    /* n^8  */
    seq_t pow_n8 = big_pow((seq_t)n, (seq_t)8);
    return pow7 - pow_n8;
}
