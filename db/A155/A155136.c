/*
 * A155136  Integers k such that k + 28 is a square.
 * https://oeis.org/A155136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 - 2*n - 27
 * Range:     i8 n <= 13, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;        /* convert index to signed type */
    return nn * nn - 2 * nn - (seq_t)27;
}
