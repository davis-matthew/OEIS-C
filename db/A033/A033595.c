/*
 * A033595  a(n) = (n^2-1)*(2*n^2-1).
 * https://oeis.org/A033595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2-1)*(2*n^2-1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;           /* n^2 */
    seq_t term1 = sq - (seq_t)1;   /* n^2 - 1 */
    seq_t term2 = (sq << 1) - (seq_t)1; /* 2*n^2 - 1 */
    return term1 * term2;
}
