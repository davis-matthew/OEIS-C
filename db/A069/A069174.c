/*
 * A069174  Centered 23-gonal numbers.
 * https://oeis.org/A069174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (23*n*n - 23*n + 2)/2
 * Range:     u8 n <= 5, u16 n <= 75, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    return ((seq_t)23 * nseq * nseq - (seq_t)23 * nseq + (seq_t)2) / (seq_t)2;
}
