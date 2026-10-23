/*
 * A153258  a(n) = n^3 - (n+2)^2.
 * https://oeis.org/A153258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - 4*n^2 + n - 2
 * Range:     i8 n <= 6, i16 n <= 33, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    return nn3 - 4 * nn2 + nn - (seq_t)2;
}
