/*
 * A158482  a(n) = 14*n^2 + 1.
 * https://oeis.org/A158482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n*n + 1
 * Range:     u8 n <= 4, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t val = nseq * nseq;          /* n^2 */
    val = val * (seq_t)14;            /* 14*n^2 */
    val = val + (seq_t)1;             /* +1 */
    return val;
}
