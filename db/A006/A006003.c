/*
 * A006003  a(n) = n*(n^2 + 1)/2.
 * https://oeis.org/A006003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n^2 + 1)/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    
    if (n_seq % 2 == 0) {
        return (n_seq / 2) * (n2 + 1);
    } else {
        return n_seq * ((n2 + 1) / 2);
    }
}
