/*
 * A168589  a(n) = (2 - 3^n)*(-1)^n.
 * https://oeis.org/A168589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2 - 3^n)*(-1)^n
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 3^n */
    seq_t pow3n = pow3(n);

    /* Compute the sign (-1)^n */
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;

    /* Compute (2 - 3^n) * sign */
    seq_t result = ((seq_t)2 - pow3n) * sign;

    return result;
}
