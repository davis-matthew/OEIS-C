/*
 * A126420  a(n) = n^3 - n - 1.
 * https://oeis.org/A126420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - n - 1
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1290, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    return N * N * N - N - 1;
}
