/*
 * A157855  a(n) = 103680000*n^2 - 46108800*n + 5126401.
 * https://oeis.org/A157855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 103680000*n^2 - 46108800*n + 5126401
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t sq    = nseq * nseq;
    seq_t part1 = sq * (seq_t)103680000;
    seq_t part2 = (seq_t)46108800 * nseq;
    return part1 - part2 + (seq_t)5126401;
}
