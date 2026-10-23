/*
 * A015088  Inverse of 1079th cyclotomic polynomial.
 * https://oeis.org/A015088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1079 < 13, otherwise 0
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is 1079–periodic with the first 13 coefficients equal to 1. */
seq_t a(idx_t n)
{
    const idx_t period = 1079;
    const idx_t ones   = 13;
    idx_t r = n % period;
    return (r < ones) ? (seq_t)1 : (seq_t)0;
}
