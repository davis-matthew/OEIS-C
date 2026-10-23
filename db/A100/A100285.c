/*
 * A100285  Expansion of (1+5*x^2)/(1-x+x^2-x^3).
 * https://oeis.org/A100285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n % 4 < 2) ? 1 : 5 (period 4)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use bitwise AND for modulo 4 on unsigned integers */
    idx_t mod4 = n & 3U;          /* remainder of n divided by 4 */
    if (mod4 < 2U)
        return (seq_t)1;          /* for n mod 4 = 0 or 1 */
    else
        return (seq_t)5;          /* for n mod 4 = 2 or 3 */
}
