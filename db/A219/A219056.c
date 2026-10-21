/*
 * A219056  a(n) = 3*n^4.
 * https://oeis.org/A219056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^4
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 194, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;   /* t = n */
    t *= t;               /* t = n^2 */
    t *= t;               /* t = n^4 */
    t *= (seq_t)3;        /* t = 3*n^4 */
    return t;
}
