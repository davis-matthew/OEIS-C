/*
 * A142463  a(n) = 2*n^2 + 2*n - 1.
 * https://oeis.org/A142463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n*n + 2*n - 1
 * Range:     i8 n <= 7, i16 n <= 127, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return 2 * nn * nn + 2 * nn - 1;
}
