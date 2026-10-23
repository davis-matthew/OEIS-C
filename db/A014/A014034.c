/*
 * A014034  Inverse of 25th cyclotomic polynomial.
 * https://oeis.org/A014034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 25 == 0, -1 if n mod 25 == 5, 0 otherwise
 *            (generating function 1/(1 + x^5 + x^10 + x^15 + x^20))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* modulo 25 is sufficient to determine the value of the series */
    idx_t m = n % 25ULL;

    if (m == 0)
        return (seq_t)1;    /* a(0), a(25), a(50), … */
    if (m == 5)
        return (seq_t)-1;   /* a(5), a(30), a(55), … */

    return (seq_t)0;        /* all other indices */
}
