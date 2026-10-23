/*
 * A015516  Inverse of 1507th cyclotomic polynomial.
 * https://oeis.org/A015516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) = 1 if (n mod 1507) < 11 else 0
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 1507;
    const idx_t cutoff = 11;
    idx_t r = n % period;
    return (r < cutoff) ? (seq_t)1 : (seq_t)0;
}
