/*
 * A005570  Number of walks on cubic lattice.
 * https://oeis.org/A005570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 8*n^2 + 9*n
 * Range:     u8 n <= 5, u16 n <= 89, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return n_seq * (8 * n_seq + 9);
}
