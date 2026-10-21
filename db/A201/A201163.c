/*
 * A201163  Expansion of g.f. 1+x+(1+3*x+x^2)/(1+x)^3.
 * https://oeis.org/A201163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n+1) * (n^2 + n - 2) / 2, a(0)=2, a(1)=1
 * Range:     i8 n <= 15, i16 n <= 255, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 1;

    /* compute (n*(n+1)-2)/2  as a positive number */
    seq_t term = ((seq_t)n * ((seq_t)n + 1) - 2) / 2;

    /* sign is positive for odd n, negative for even n */
    if ((n & 1) != 0)
        return term;
    else
        return -term;
}
