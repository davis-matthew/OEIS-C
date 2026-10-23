/*
 * A147973  a(n) = -2*n^2 + 12*n - 14.
 * https://oeis.org/A147973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*n^2 + 12*n - 14
 * Range:     i8 n <= 11, i16 n <= 131, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return -2 * nn * nn + 12 * nn - 14;
}
