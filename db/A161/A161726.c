/*
 * A161726  a(n) = n^2 - 917*n + 9479.
 * https://oeis.org/A161726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n*n - 917*n + 9479
 * Range:     i8 none, i16 n <= 48, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * nn - (seq_t)917 * nn + (seq_t)9479;
}
