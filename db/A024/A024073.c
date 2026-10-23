/*
 * A024073  a(n) = 6^n - n^11.
 * https://oeis.org/A024073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   pow_uint, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pow6 = pow_uint(6, n);

    /* n^11 */
    seq_t pown = seq_pow((seq_t)n, 11);

    /* a(n) = 6^n - n^11 */
    return pow6 - pown;
}
