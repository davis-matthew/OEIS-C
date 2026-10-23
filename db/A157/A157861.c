/*
 * A157861  a(n) = 3600*n^2 + n.
 * https://oeis.org/A157861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3600*n^2 + n
 * Range:     u8 none, u16 n <= 4, u32 n <= 1092, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* value for n < 1 is irrelevant */
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;   /* n^2   */
    res *= 3600;           /* 3600*n^2 */
    res += nn;             /* + n   */
    return res;
}
