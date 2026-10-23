/*
 * A014025  Expansion of the inverse of the 16th cyclotomic polynomial.
 * https://oeis.org/A014025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n mod 8 == 0 ? (-1)^(n/8) : 0)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014025: expansion of 1/(1+x^8) */

seq_t a(idx_t n)
{
    /* zero unless n is a multiple of 8 */
    if ((n & 7) != 0)
        return 0;

    /* k = n/8.  Even k gives +1, odd k gives -1 */
    idx_t k = n >> 3;
    return (k & 1) ? (seq_t)-1 : (seq_t)1;
}
