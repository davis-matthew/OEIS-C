/*
 * A014386  Inverse of 377th cyclotomic polynomial.
 * https://oeis.org/A014386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 if n mod 377 <= 12; -1 if 29 <= n mod 377 <= 41; 0
 *            otherwise This is derived from 1/Φ_377(x) = P(x)/(1-x^377) where
 *            P(x) = -(x-1)*Φ_13(x)*Φ_29(x) has coefficients p_0..p_12 = 1,
 *            p_29..p_41 = -1, all other p_j = 0
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period 377 */
    const idx_t r = n % 377;

    if (r <= 12) {
        return (seq_t)1;
    } else if (r <= 28) {
        return (seq_t)0;
    } else if (r <= 41) {
        return (seq_t)(-1);
    } else {
        return (seq_t)0;
    }
}
