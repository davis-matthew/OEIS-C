/*
 * A157443  a(n) = 121*n^2 - 38*n + 3.
 * https://oeis.org/A157443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 121*n^2 - 38*n + 3
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 5957, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term = (seq_t)121 * nseq * nseq - (seq_t)38 * nseq + (seq_t)3;
    return term;
}
