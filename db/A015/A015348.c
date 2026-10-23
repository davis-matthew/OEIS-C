/*
 * A015348  Inverse of 1339th cyclotomic polynomial.
 * https://oeis.org/A015348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1339 < 13; 0 otherwise (periodic with period
 *            1339)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t P = 1339;
    return (seq_t)((n % P) < 13);
}
