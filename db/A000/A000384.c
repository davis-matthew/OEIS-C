/*
 * A000384  Hexagonal numbers: a(n) = n*(2*n-1).
 * https://oeis.org/A000384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(2*n-1)
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return n_seq * (2 * n_seq - 1);
}
