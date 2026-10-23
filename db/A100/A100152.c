/*
 * A100152  Structured truncated cubic numbers.
 * https://oeis.org/A100152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(31*n^2 - 27*n + 2)/6
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 626, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term = (seq_t)31 * nseq * nseq - (seq_t)27 * nseq + (seq_t)2;
    return (nseq * term) / 6;
}
