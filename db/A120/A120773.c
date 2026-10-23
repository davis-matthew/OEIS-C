/*
 * A120773  a(n) = 2^(n^2) - 3^n.
 * https://oeis.org/A120773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2^(n^2) - 3^n
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 7, i128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t a2 = big_pow((seq_t)2, sq);
    seq_t a3 = big_pow((seq_t)3, (seq_t)n);
    return a2 - a3;
}
