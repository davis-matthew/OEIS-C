/*
 * A033569  a(n) = (2*n - 1)*(3*n + 1).
 * https://oeis.org/A033569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n - 1)*(3*n + 1)
 * Range:     i8 n <= 4, i16 n <= 73, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return (seq_t)((2*nn - 1) * (3*nn + 1));
}
