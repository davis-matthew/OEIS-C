/*
 * A154361  a(n) = 250*n - 70.
 * https://oeis.org/A154361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 250*n - 70
 * Range:     i8 n <= 0, i16 n <= 131, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * 250 - 70;
}
