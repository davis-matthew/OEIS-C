/*
 * A062765  a(n) = n*(n-1)*(n-3)*(n-5).
 * https://oeis.org/A062765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n-1)*(n-3)*(n-5)
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * (sn - 1) * (sn - 3) * (sn - 5);
}
