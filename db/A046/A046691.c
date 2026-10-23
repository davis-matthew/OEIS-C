/*
 * A046691  a(n) = (n^2 + 5*n - 2)/2.
 * https://oeis.org/A046691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n+5)/2 - 1
 * Range:     i8 n <= 13, i16 n <= 253, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * (nn + 5) / 2) - 1;
}
