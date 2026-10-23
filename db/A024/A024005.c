/*
 * A024005  a(n) = 1 - n^7.
 * https://oeis.org/A024005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 21, i64 n <= 450, i128 n <= 450,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first. */
    seq_t r = (seq_t)n;

    /* Compute r^7 using only seq_t arithmetic. */
    seq_t r2 = r * r;            /* r^2          */
    seq_t r4 = r2 * r2;          /* r^4          */
    seq_t r6 = r4 * r2;          /* r^6 = r^4 * r^2 */
    seq_t r7 = r6 * r;           /* r^7 = r^6 * r */

    /* a(n) = 1 - n^7 */
    return 1 - r7;
}
