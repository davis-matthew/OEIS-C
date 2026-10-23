/*
 * A014022  Inverse of 13th cyclotomic polynomial.
 * https://oeis.org/A014022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 for n mod 13 = 0, a(n) = -1 for n mod 13 = 1, otherwise
 *            0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 13;
    if (r == 0) return (seq_t)1;
    if (r == 1) return (seq_t)-1;
    return (seq_t)0;
}
