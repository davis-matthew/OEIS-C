/*
 * A011379  a(n) = n^2*(n+1).
 * https://oeis.org/A011379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;     /* convert index to seq_t */
    seq_t n_sq = nn * nn;    /* n^2 */
    return n_sq * (nn + 1);  /* n^2 * (n+1) */
}
