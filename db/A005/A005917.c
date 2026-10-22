/*
 * A005917  Rhombic dodecahedral numbers: a(n) = n^4 - (n - 1)^4.
 * https://oeis.org/A005917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^4 - (n - 1)^4
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1024, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_minus_1 = (seq_t)(n - 1);
    seq_t n4 = n_seq * n_seq * n_seq * n_seq;
    seq_t nm1_4 = n_minus_1 * n_minus_1 * n_minus_1 * n_minus_1;
    return n4 - nm1_4;
}
