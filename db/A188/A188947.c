/*
 * A188947  a(n) = n^3 - 2*n^2 + 2*n + 1.
 * https://oeis.org/A188947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^2 + (n+1) = n^3 - 2*n^2 + 2*n + 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n1 = nn - 1;            /* n-1  */
    seq_t n1sq = n1 * n1;         /* (n-1)^2 */
    seq_t res = nn * n1sq;        /* n*(n-1)^2 */
    res += nn + 1;                /* add n + 1 */
    return res;
}
