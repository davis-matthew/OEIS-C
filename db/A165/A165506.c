/*
 * A165506  a(0) = 1, a(1) = 8, a(n) = 56*a(n-2) - a(n-1).
 * https://oeis.org/A165506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 56*a(n-2) – a(n-1); a(0) = 1, a(1) = 8
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 22, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    /* Iteratively compute the recurrence. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)8;   /* a(1) */
    seq_t an = 0;          /* placeholder for a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)56 * a0 - a1;
        a0 = a1;
        a1 = an;
    }
    return an;
}
