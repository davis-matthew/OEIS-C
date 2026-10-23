/*
 * A024152  a(n) = 12^n - n^12.
 * https://oeis.org/A024152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 17, i128 n <= 35,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow12_n = big_pow((seq_t)12, (seq_t)n);
    seq_t n_pow12 = big_pow((seq_t)n, (seq_t)12);
    return pow12_n - n_pow12;
}
