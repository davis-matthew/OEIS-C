/*
 * A087960  a(n) = (-1)^binomial(n+1,2).
 * https://oeis.org/A087960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^floor((n+1)/2) (period 4: 1, -1, -1, 1)
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute floor((n+1)/2) and check its parity */
    idx_t m = (n + 1) >> 1;
    return (m & 1) ? (seq_t)(-1) : (seq_t)1;
}
