/*
 * A211607  a(n) = 111*n^2 - 3123*n + 10753.
 * https://oeis.org/A211607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 111*n^2 - 3123*n + 10753
 * Range:     i8 none, i16 n <= 33, i32 n <= 39, i64 n <= 39, i128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t for safe arithmetic */
    seq_t nn = n;
    /* Compute the formula using seq_t arithmetic only */
    seq_t result = (seq_t)111 * nn * nn
                 - (seq_t)3123 * nn
                 + (seq_t)10753;
    return result;
}
