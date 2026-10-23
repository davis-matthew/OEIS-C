/*
 * A024138  a(n) = 11^n - n^11.
 * https://oeis.org/A024138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 18, i128 n <= 36,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Compute 11^n */
    seq_t pow_11 = pow_int((seq_t)11, n);

    /* Compute n^11 */
    seq_t pow_n_11 = pow_int((seq_t)n, (idx_t)11);

    /* Return the difference */
    return pow_11 - pow_n_11;
}
