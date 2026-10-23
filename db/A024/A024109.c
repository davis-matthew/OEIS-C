/*
 * A024109  a(n) = 9^n - n^8.
 * https://oeis.org/A024109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 9^n - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 19, i128 n <= 40,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute 9^n */
    seq_t pow9 = pow_uint(9, n);      /* 9^n */

    /* compute n^8 */
    seq_t pow8 = (seq_t)1;
    seq_t sn   = (seq_t)n;
    for (idx_t i = 0; i < 8; ++i)
        pow8 *= sn;                  /* n^8 */

    return pow9 - pow8;
}
