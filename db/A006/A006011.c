/*
 * A006011  a(n) = n^2*(n^2 - 1)/4.
 * https://oeis.org/A006011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2 - 1)/4
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 362, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) {
        return 0;
    }
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    if ((n2 & 3) == 0) {
        return (n2 / 4) * (n2 - 1);
    } else {
        return n2 * ((n2 - 1) / 4);
    }
}
