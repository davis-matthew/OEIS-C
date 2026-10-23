/*
 * A023469  a(n) = n-27.
 * https://oeis.org/A023469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n-27
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n - 27;
}
