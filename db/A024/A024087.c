/*
 * A024087  a(n) = 7^n - n^12.
 * https://oeis.org/A024087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 22, i128 n <= 45,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow7  = seq_pow((seq_t)7,   n);
    seq_t pow12 = seq_pow((seq_t)n, 12);
    return pow7 - pow12;
}
