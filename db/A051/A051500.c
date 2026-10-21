/*
 * A051500  a(n) = (3^n + 1)^2/4.
 * https://oeis.org/A051500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n + 1)^2/4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t w = 1;                 /* w = 3^n */
    for (idx_t i = 0; i < n; ++i)
        w *= (seq_t)3;           /* multiply by small constant */

    seq_t q = (w + (seq_t)1) / (seq_t)2;   /* q = (3^n + 1) / 2 */
    return q * q;                                 /* a(n) = q^2 */
}
