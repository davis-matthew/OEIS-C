/*
 * A024137  a(n) = 11^n - n^10.
 * https://oeis.org/A024137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11^n - n^10
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 9, i64 n <= 18, i128 n <= 36,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t power11 = big_pow((seq_t)11, (seq_t)n);
    seq_t power10 = big_pow((seq_t)n, (seq_t)10);
    return power11 - power10;
}
