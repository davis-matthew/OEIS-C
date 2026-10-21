/*
 * A222740  Denominators of 1/16 - 1/(4 + 8*n)^2.
 * https://oeis.org/A222740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = ((n mod 4 == 1 or 2) ? 2 : 1) * (2*n+1)^2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* coefficient is 2 for n ≡ 1 or 2 (mod 4), otherwise 1 */
    seq_t coeff = ((n % 4 == 1 || n % 4 == 2) ? (seq_t)2 : (seq_t)1);

    /* x = 2*n + 1, computed in seq_t to avoid overflow */
    seq_t x = (seq_t)2 * (seq_t)n + (seq_t)1;

    /* return coefficient * x^2 */
    return coeff * x * x;
}
