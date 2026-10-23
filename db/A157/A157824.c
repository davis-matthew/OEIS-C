/*
 * A157824  a(n) = 3600*n^2 - 6751*n + 3165.
 * https://oeis.org/A157824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3600*n^2 - 6751*n + 3165
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 1093, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; values for n = 0 are unused. */
    if (n == 0) return (seq_t)0;
    seq_t nseq = (seq_t)n;
    seq_t sq = nseq * nseq;
    seq_t result = (seq_t)3600 * sq - (seq_t)6751 * nseq + (seq_t)3165;
    return result;
}
