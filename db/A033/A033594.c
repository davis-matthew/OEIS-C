/*
 * A033594  a(n) = (n-1)*(2*n-1)*(3*n-1).
 * https://oeis.org/A033594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n-1)*(2*n-1)*(3*n-1)
 * Range:     i8 n <= 3, i16 n <= 18, i32 n <= 710, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset: 0 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn - 1;          /* n-1 */
    seq_t t2 = nn * 2 - 1;      /* 2*n-1 */
    seq_t t3 = nn * 3 - 1;      /* 3*n-1 */
    return t1 * t2 * t3;
}
