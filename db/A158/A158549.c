/*
 * A158549  a(n) = 26*n^2 + 1.
 * https://oeis.org/A158549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*n^2 + 1
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* convert n to seq_t */
    seq_t sq = nn * nn;            /* n^2 */
    return 26 * sq + 1;            /* 26*n^2 + 1 */
}
