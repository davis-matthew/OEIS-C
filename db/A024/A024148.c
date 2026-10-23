/*
 * A024148  a(n) = 12^n - n^8.
 * https://oeis.org/A024148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 12^n - n^8
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 8, i64 n <= 17, i128 n <= 35,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 12^n */
    seq_t pow12 = seq_pow((seq_t)12, n);

    /* n^8 */
    seq_t pow8 = seq_pow((seq_t)n, (idx_t)8);

    return pow12 - pow8;
}
