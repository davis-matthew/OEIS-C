/*
 * A005582  a(n) = n*(n+1)*(n+2)*(n+7)/24.
 * https://oeis.org/A005582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+7)/24
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 563, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return n_seq * (n_seq + 1) * (n_seq + 2) * (n_seq + 7) / 24;
}
