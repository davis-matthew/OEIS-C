/*
 * A005581  a(n) = (n-1)*n*(n+4)/6.
 * https://oeis.org/A005581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*n*(n+4)/6
 * Range:     u8 n <= 10, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t half_prod = (n_seq % 2 == 0) ? (n_seq - 1) * (n_seq / 2) : ((n_seq - 1) / 2) * n_seq;
    return half_prod * (n_seq + 4) / 3;
}
