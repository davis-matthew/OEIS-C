/*
 * A131471  a(n) = n^5+n.
 * https://oeis.org/A131471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t x2 = x * x;         /* n^2 */
    seq_t x3 = x2 * x;        /* n^3 */
    seq_t x4 = x3 * x;        /* n^4 */
    seq_t x5 = x4 * x;        /* n^5 */
    return x5 + x;            /* n^5 + n */
}
