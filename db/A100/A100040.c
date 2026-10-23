/*
 * A100040  a(n) = 2*n^2 + n - 5.
 * https://oeis.org/A100040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n^2 + n - 5
 * Range:     i8 n <= 7, i16 n <= 127, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    return 2 * k * k + k - 5;
}
