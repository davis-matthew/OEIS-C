/*
 * A152017  a(n) = n^5-n^4-n^3-n^2-n.
 * https://oeis.org/A152017
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^5 - n^4 - n^3 - n^2 - n
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 73, i64 n <= 6208,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the terms of OEIS A152017 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast n to seq_t to preserve exact integer arithmetic */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t n4 = n3 * sn;          /* n^4 */
    seq_t n5 = n4 * sn;          /* n^5 */
    return n5 - n4 - n3 - n2 - sn;
}
