/*
 * A131478  a(n) = ceiling(n^4/4).
 * https://oeis.org/A131478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^4 / 4)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 361, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;     /* n^2 */
    seq_t n4 = n2 * n2;     /* n^4 */
    seq_t div = n4 >> 2;    /* floor(n^4/4) */
    seq_t rem = n4 & 3;     /* n^4 mod 4 */
    return div + (seq_t)(rem != 0); /* ceil */
}
