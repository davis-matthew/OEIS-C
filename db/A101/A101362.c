/*
 * A101362  a(n) = (n+1)*n^4.
 * https://oeis.org/A101362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n+1)*n^4
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;      /* cast to seq_t first */
    seq_t n2 = nn * nn;      /* n^2 */
    seq_t n4 = n2 * n2;      /* n^4 */
    seq_t res = (nn + 1) * n4;
    return res;
}
