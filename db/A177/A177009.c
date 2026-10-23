/*
 * A177009  a(n) = n^n - A002275(n).
 * https://oeis.org/A177009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, base
 * Formula:   a(n) = n^n - (10^n - 1)/9
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 9, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Promote n to seq_t for all arithmetic */
    seq_t ns = (seq_t)n;

    /* Compute n^n */
    seq_t pow_n = pow_int(ns, n);

    /* Compute 10^n */
    seq_t pow10_n = pow_int((seq_t)10, n);

    /* Compute repunit (10^n - 1)/9 */
    seq_t repunit = (pow10_n - (seq_t)1) / (seq_t)9;

    /* Return the difference */
    return pow_n - repunit;
}
