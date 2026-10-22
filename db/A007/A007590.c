/*
 * A007590  a(n) = floor(n^2/2).
 * https://oeis.org/A007590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = floor(n^2/2)
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return (n_seq * n_seq) / 2;
}
