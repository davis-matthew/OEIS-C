/*
 * A029153  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^6)*(1-x^10)).
 * https://oeis.org/A029153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+36*n^2+468*n+2592)/2160 - * (n^2+21*n)*(n mod
 *            2)/240 - * (n+9)*((n^2) mod 3)/18)
 * Range:     u8 none, u16 n <= 29, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Numerator of the fraction with common denominator 2160 */
    seq_t term = nn * nn * nn;          /* n^3 */
    term += 36 * nn * nn;               /* 36 n^2 */
    term += 468 * nn;                   /* 468 n */
    term += 2592;                       /* constant */

    /* Subtract (n^2+21*n)*(n mod 2)/240, multiplied by 9 to same denom */
    term -= 9 * ((nn * nn + 21 * nn) * (n & 1));

    /* Subtract (n+9)*((n^2) mod 3)/18, multiplied by 120 to same denom */
    term -= 120 * ((nn + 9) * ((nn * nn) % 3));

    /* Divide by 2160 to obtain the integer floor value */
    return term / 2160;
}
