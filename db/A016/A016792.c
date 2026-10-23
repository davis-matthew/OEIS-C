/*
 * A016792  a(n) = (3*n+2)^4.
 * https://oeis.org/A016792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+2)^4
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 84, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute x = 3*n + 2 */
    seq_t x = ((seq_t)3 * n) + (seq_t)2;
    /* y = x^2 */
    seq_t y = x * x;
    /* return y^2 = x^4 */
    return y * y;
}
