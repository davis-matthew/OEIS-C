/*
 * A015538  Inverse of 1529th cyclotomic polynomial.
 * https://oeis.org/A015538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1529 ≤ 10, otherwise 0 (periodic with period
 *            1529)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 1 for the first 11 residues modulo 1529, else 0 */
    return (n % 1529ULL <= 10ULL) ? 1 : 0;
}
