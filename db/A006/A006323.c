/*
 * A006323  4-dimensional analog of centered polygonal numbers.
 * https://oeis.org/A006323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n^2 + 7*n - 2)/24
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 292, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t m = n_seq * (n_seq + 1) / 2;
    return m * (7 * m - 1) / 6;
}
