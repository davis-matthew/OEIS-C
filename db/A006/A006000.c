/*
 * A006000  a(n) = (n+1)*(n^2+n+2)/2.
 * https://oeis.org/A006000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2+n+2)/2
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t triangular = n_seq * (n_seq + 1) / 2;
    return (n_seq + 1) * (triangular + 1);
}
