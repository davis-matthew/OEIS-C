/*
 * A006004  a(n) = C(n+2,3) + C(n,3) + C(n-1,3).
 * https://oeis.org/A006004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n^3 - 2n^2 + 5n - 2) / 2
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    return (n3 - 2 * n2 + 5 * n_seq - 2) / 2;
}
