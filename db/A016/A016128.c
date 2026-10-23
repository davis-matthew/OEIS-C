/*
 * A016128  Inverse of 2119th cyclotomic polynomial.
 * https://oeis.org/A016128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if (n mod 2119) < 13, otherwise 0
 * Range:     i8 n <= 3000, i16 n <= 3000, i32 n <= 3000, i64 n <= 3000,
 *            i128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic with period length 2119. */
seq_t a(idx_t n)
{
    const idx_t period = 2119ULL;   /* period length */
    const idx_t thresh = 13ULL;     /* first 13 terms are 1 */
    idx_t r = n % period;          /* r is in [0, period-1] */
    return (r < thresh) ? (seq_t)1 : (seq_t)0;
}
