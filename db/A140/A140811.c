/*
 * A140811  a(n) = 6*n^2 - 1.
 * https://oeis.org/A140811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*n^2 - 1
 * Range:     i8 n <= 4, i16 n <= 73, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)6 * nn * nn) - 1;
}
