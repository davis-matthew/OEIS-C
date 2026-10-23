/*
 * A023471  a(n) = n-29.
 * https://oeis.org/A023471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n-29
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n - 29;
}
