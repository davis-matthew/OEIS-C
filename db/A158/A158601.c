/*
 * A158601  a(n) = 400*n^2 + 20.
 * https://oeis.org/A158601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 400*n^2 + 20
 * Range:     u8 n <= 0, u16 n <= 12, u32 n <= 3276, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    return (seq_t)400 * nseq * nseq + (seq_t)20;
}
