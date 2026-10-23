/*
 * A196738  a(n) = n! - n^8.
 * https://oeis.org/A196738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! - n^8
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact = fact * (seq_t)i;
    }
    seq_t pow8 = 1;
    for (int k = 0; k < 8; ++k) {
        pow8 = pow8 * (seq_t)n;
    }
    return fact - pow8;
}
