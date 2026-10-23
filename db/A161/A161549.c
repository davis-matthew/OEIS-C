/*
 * A161549  a(n) = 2*n^2 + 14*n + 1.
 * https://oeis.org/A161549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n^2 + 14*n + 1
 * Range:     u8 n <= 8, u16 n <= 177, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t res = tn * tn;   /* tn^2 */
    res *= 2;              /* 2*tn^2 */
    res += tn * 14;        /* +14*n */
    res += 1;              /* +1 */
    return res;
}
