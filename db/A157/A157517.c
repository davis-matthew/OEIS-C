/*
 * A157517  a(n) = 7 + 12*n - 6*n^2.
 * https://oeis.org/A157517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 7 + 12*n - 6*n^2
 * Range:     i8 n <= 5, i16 n <= 74, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    return (seq_t)7 + (seq_t)12 * N - (seq_t)6 * N * N;
}
