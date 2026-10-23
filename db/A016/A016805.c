/*
 * A016805  a(n) = (4*n)^5.
 * https://oeis.org/A016805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n)^5
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 21, u64 n <= 1782,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x *= 4;                     /* x = 4*n */
    seq_t res = 1;
    for (idx_t i = 0; i < 5; ++i)
        res *= x;               /* res = (4*n)^5 */
    return res;
}
