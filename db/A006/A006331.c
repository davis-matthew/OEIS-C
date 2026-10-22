/*
 * A006331  a(n) = n*(n+1)*(2*n+1)/3.
 * https://oeis.org/A006331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(2*n+1)/3
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 1822, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return n_seq * (n_seq + 1) * (2 * n_seq + 1) / 3;
}
