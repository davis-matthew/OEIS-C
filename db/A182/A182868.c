/*
 * A182868  a(n) = -1 + n + 4*n^2.
 * https://oeis.org/A182868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -1 + n + 4*n^2
 * Range:     i8 n <= 5, i16 n <= 49, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return 4 * x * x + x - 1;
}
