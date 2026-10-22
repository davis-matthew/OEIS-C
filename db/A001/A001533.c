/*
 * A001533  a(n) = (8*n+1)*(8*n+7).
 * https://oeis.org/A001533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+1)*(8*n+7)
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return (8 * n_seq + 1) * (8 * n_seq + 7);
}
