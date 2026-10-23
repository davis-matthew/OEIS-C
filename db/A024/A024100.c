/*
 * A024100  a(n) = 8^n - n^12.
 * https://oeis.org/A024100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 21, i128 n <= 42,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 8^n and n^12 using the generic power function */
    seq_t pow8  = seq_pow((seq_t)8, n);
    seq_t pow12 = seq_pow((seq_t)n, 12);

    /* Return the difference as specified */
    return pow8 - pow12;
}
