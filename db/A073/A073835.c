/*
 * A073835  Replace 10^k with (-10)^k in decimal expansion of n.
 * https://oeis.org/A073835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = sum_{i=0}^{k} d_i * (-10)^i where n = d_k d_{k-1} ... d_0
 *            in ordinary decimal notation. This is the same as interpreting
 *            the decimal digits of n as a number in base -10
 * Range:     i8 n <= 60, i16 n <= 60, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;          /* (-10)^0 initially */
    while (n > 0)
    {
        idx_t digit = n % 10;          /* current least‑significant decimal digit */
        result += (seq_t)digit * power;/* add digit * (-10)^current_exponent */
        power  *= (seq_t)-10;          /* advance exponent: multiply by -10 */
        n /= 10;                       /* move to next digit */
    }
    return result;
}
