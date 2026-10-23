/*
 * A136316  a(n) = 13 + 12*n - n^2.
 * https://oeis.org/A136316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 13 + 12*n - n^2
 * Range:     i8 n <= 19, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return 13 + 12 * tn - tn * tn;
}
