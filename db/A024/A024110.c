/*
 * A024110  a(n) = 9^n - n^9.
 * https://oeis.org/A024110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow9n = big_pow((seq_t)9, (seq_t)n);    /* 9^n   */
    seq_t pown9 = big_pow((seq_t)n, (seq_t)9);    /* n^9   */
    return pow9n - pown9;
}
