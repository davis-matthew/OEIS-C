/*
 * A001505  a(n) = (4*n+1)*(4*n+2)*(4*n+3).
 * https://oeis.org/A001505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+1)*(4*n+2)*(4*n+3)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 405, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t f1 = 4 * n_seq + 1;
    seq_t f2 = 4 * n_seq + 2;
    seq_t f3 = 4 * n_seq + 3;
    return f1 * f2 * f3;
}
