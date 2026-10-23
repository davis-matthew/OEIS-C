/*
 * A015946  Inverse of 1937th cyclotomic polynomial.
 * https://oeis.org/A015946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1937 < 13, otherwise 0 (periodic with period
 *            1937)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n modulo 1937 */
    idx_t r = n % 1937;
    /* return 1 for residues 0..12, 0 otherwise */
    return (r < 13) ? 1 : 0;
}
