/*
 * A016949  a(n) = (6*n + 3)^5.
 * https://oeis.org/A016949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3)^5
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 13, u64 n <= 1188,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = (seq_t)6 * nn + 3;
    seq_t tt = t * t;       /* t^2 */
    seq_t ttt = tt * tt;    /* t^4 */
    seq_t res = ttt * t;    /* t^5 */
    return res;
}
