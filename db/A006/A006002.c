/*
 * A006002  a(n) = n*(n+1)^2/2.
 * https://oeis.org/A006002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*(n+1)^2/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_plus_1 = n_seq + 1;
    
    if (n_seq & 1) {
        return n_seq * (n_plus_1 / 2) * n_plus_1;
    } else {
        return (n_seq / 2) * n_plus_1 * n_plus_1;
    }
}
