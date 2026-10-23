/*
 * A015748  Inverse of 1739th cyclotomic polynomial.
 * https://oeis.org/A015748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 1739 in [0,36], a(n) = -1 if n mod 1739 in
 *            [47,83], a(n) = 0 otherwise. This is the periodic sequence of
 *            the inverse (power‑series expansion) of the 1739th cyclotomic
 *            polynomial
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const idx_t period = 1739;
    idx_t m = n % period;

    if (m <= 36)           /* 0 … 36 : 1   */
        return (seq_t)1;
    if (m >= 47 && m <= 83)/* 47 … 83 : –1 */
        return (seq_t)-1;
    /* all other residues: 0 */
    return (seq_t)0;
}
