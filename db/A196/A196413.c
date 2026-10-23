/*
 * A196413  a(n) = n! - n^6.
 * https://oeis.org/A196413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = n! - n^6
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    seq_t pow6 = (seq_t)n;
    for (int i = 1; i < 6; ++i) {
        pow6 *= (seq_t)n;
    }

    return fact - pow6;
}
