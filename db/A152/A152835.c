/*
 * A152835  a(0) = -22; a(n) = n-a(n-1).
 * https://oeis.org/A152835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = -22; for n > 0, a(n) = n - a(n-1). Equivalent closed
 *            form: if n is even: a(n) = -22 + n/2 if n is odd : a(n) = 22 +
 *            (n+1)/2
 * Range:     i8 n <= 209, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to seq_t after shifting to compute the floor division.
       Using shift on the casted value keeps the operation within seq_t. */
    seq_t half = ((seq_t)n) >> 1;   /* floor(n/2) as seq_t */

    if ((n & 1) == 0) {
        /* even n: -22 + n/2 */
        return (seq_t)(-22) + half;
    } else {
        /* odd n:  22 + (n+1)/2  -> half + 1 is (n+1)/2 */
        return (seq_t)22 + (half + 1);
    }
}
