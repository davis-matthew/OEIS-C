/*
 * A015250  Inverse of 1241st cyclotomic polynomial.
 * https://oeis.org/A015250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 when n mod 1241 is in [0,16], * a(n) = -1 when n mod
 *            1241 is in [73,89], * a(n) = 0 otherwise. * This follows from
 *            1/Φ₁₂₄₁(x)=((x⁷⁸-1)(1+x+…+x⁷²))/(x¹²⁴¹-1)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 1241;          /* periodic reduction */
    if (r <= 16) return 1;      /* coefficients of -x^k for 0≤k≤16 */
    if (r <= 72) return 0;      /* zero gap from 17 to 72 */
    if (r <= 89) return -1;     /* coefficients +x^k for 73≤k≤89 */
    return 0;                   /* zero for the rest of the period */
}
