/*
 * A023449  a(n) = n-7.
 * https://oeis.org/A023449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n-7
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n - 7;
}
