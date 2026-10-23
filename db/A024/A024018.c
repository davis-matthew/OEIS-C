/*
 * A024018  a(n) = 2^n-n^8.
 * https://oeis.org/A024018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 14, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert index to sequence type */
    seq_t sn = (seq_t)n;

    /* Compute n^8 via successive squaring */
    seq_t n2 = sn * sn;     /* n^2   */
    seq_t n4 = n2 * n2;     /* n^4   */
    seq_t n8 = n4 * n4;     /* n^8   */

    /* Compute 2^n using the provided utility */
    seq_t two_n = pow2(n);

    /* Return the difference */
    return two_n - n8;
}
