/*
 * A002412  Hexagonal pyramidal numbers, or greengrocer's numbers.
 * https://oeis.org/A002412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(4*n - 1)/6
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 1636, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t half = nseq * (nseq + 1) / 2;
    return (half * (4 * nseq - 1)) / 3;
}
