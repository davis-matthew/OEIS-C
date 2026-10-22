/*
 * A000466  a(n) = 4*n^2 - 1.
 * https://oeis.org/A000466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n^2 - 1
 * Range:     i8 n <= 5, i16 n <= 90, i32 n <= 900, i64 n <= 900,
 *            i128 n <= 900, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    return 4 * m * m - 1;
}
