/*
 * A016816  a(n) = (4n+1)^4.
 * https://oeis.org/A016816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+1)^4
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t four = (seq_t)4;
    seq_t one  = (seq_t)1;
    seq_t x = (four * (seq_t)n) + one;   /* x = 4*n + 1 */
    seq_t y = x * x;                     /* y = x^2 */
    seq_t z = y * y;                     /* z = y^2 = (4n+1)^4 */
    return z;
}
