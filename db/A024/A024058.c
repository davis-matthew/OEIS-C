/*
 * A024058  a(n) = 5^n - n^9.
 * https://oeis.org/A024058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow5 = seq_pow((seq_t)5, n);        /* 5^n */
    seq_t pow9 = seq_pow((seq_t)n, 9);        /* n^9 */
    return pow5 - pow9;
}
