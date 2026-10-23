/*
 * A161702  a(n) = (-n^3 + 9n^2 - 5n + 3)/3.
 * https://oeis.org/A161702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-n^3 + 9n^2 - 5n + 3)/3
 * Range:     i8 n <= 9, i16 n <= 35, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_t = n;
    seq_t val = -(n_t * n_t * n_t) + 9 * n_t * n_t - 5 * n_t + 3;
    return val / 3;
}
