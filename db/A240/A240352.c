/*
 * A240352  Inverse of 67th cyclotomic polynomial.
 * https://oeis.org/A240352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 67 == 0, -1 if n mod 67 == 1, otherwise 0. The
 *            sequence is periodic with period 67
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 67U;
    if (r == 0U) return (seq_t)1;
    if (r == 1U) return (seq_t)(-1);
    return (seq_t)0;
}
