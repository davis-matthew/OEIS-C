/*
 * A003185  a(n) = (4*n+1)*(4*n+5).
 * https://oeis.org/A003185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+1)*(4*n+5)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t term1 = 4 * n_seq + 1;
    seq_t term2 = 4 * n_seq + 5;
    return term1 * term2;
}
