/*
 * A121670  a(n) = n^3 - 3*n.
 * https://oeis.org/A121670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - 3*n
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn * nn - (seq_t)3 * nn;
}
