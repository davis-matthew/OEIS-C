/*
 * A023472  a(n) = n - 30.
 * https://oeis.org/A023472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 30
 * Range:     i8 n <= 157, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n - (seq_t)30;
}
