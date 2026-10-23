/*
 * A014064  Coefficients of the reciprocal of the 55th cyclotomic polynomial.
 * https://oeis.org/A014064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (5*w - 1)*(w - 2)*(w - 3)*(w - 4) *(63*m^4 - 350*m^3 +
 *            630*m^2 - 325*m + 12) *(m - 5)! / (43545600*(m - 11)!), where m
 *            = n mod 11 and w = floor(n/11) mod 5. The sequence has period 55
 *            and only takes the values -1, 0, 1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified implementation based on the fact that the terms are
   +1 when w == 0 and m < 5, -1 when w == 1 and m < 5,
   and 0 otherwise. */
seq_t a(idx_t n)
{
    idx_t m = n % 11;
    idx_t w = (n / 11) % 5;

    if ((w == 0 || w == 1) && m <= 4)
        return (seq_t)(w == 0 ? 1 : -1);

    return (seq_t)0;
}
