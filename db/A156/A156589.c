/*
 * A156589  a(n) = 4^n - 2^n - 1.
 * https://oeis.org/A156589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - 2^n - 1
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t pow4 = seq_pow((seq_t)4, n);
    seq_t pow2 = seq_pow((seq_t)2, n);
    return pow4 - pow2 - (seq_t)1;
}
