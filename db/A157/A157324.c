/*
 * A157324  a(n) = 36*n^2 + n.
 * https://oeis.org/A157324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*n^2 + n
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;       /* convert to seq_t to avoid overflow on idx_t */
    seq_t sq = nn * nn;        /* n^2 */
    return 36 * sq + nn;       /* 36*n^2 + n */
}
