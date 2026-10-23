/*
 * A141530  a(n) = 4*n^3 - 6*n^2 + 1.
 * https://oeis.org/A141530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, less, easy
 * Formula:   a(n) = 4*n^3 - 6*n^2 + 1
 * Range:     i8 n <= 3, i16 n <= 20, i32 n <= 500, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    return 4 * x3 - 6 * x2 + 1;
}
