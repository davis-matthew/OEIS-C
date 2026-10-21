/*
 * A239794  a(n) = 5*n^2 + 4*n - 15.
 * https://oeis.org/A239794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*n^2 + 4*n - 15
 * Range:     i8 n <= 4, i16 n <= 80, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t tn = (seq_t)n;
    return 5 * tn * tn + 4 * tn - 15;
}
