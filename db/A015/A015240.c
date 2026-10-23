/*
 * A015240  a(n) = (2*n - 5)n^2.
 * https://oeis.org/A015240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n-5)*n^2
 * Range:     i8 n <= 5, i16 n <= 26, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return ((seq_t)2 * nn - (seq_t)5) * nn * nn;
}
