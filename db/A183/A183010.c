/*
 * A183010  a(n) = 24*n - 1.
 * https://oeis.org/A183010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 24*n - 1
 * Range:     i8 n <= 5, i16 n <= 1365, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)24 * (seq_t)n - 1;
}
