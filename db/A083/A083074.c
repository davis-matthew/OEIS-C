/*
 * A083074  a(n) = n^3 - n^2 - n - 1.
 * https://oeis.org/A083074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = n^3 - n^2 - n - 1
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 780, i64 n <= 780,
 *            i128 n <= 780, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    return x3 - x2 - x - 1;
}
