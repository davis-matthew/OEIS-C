/*
 * A006469  Number of rooted toroidal maps with 2 faces, n vertices and no
 *          isthmuses.
 * https://oeis.org/A006469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (474 + 1247*n + 1215*n^2 + 545*n^3 + 111*n^4 + 8*n^5)
 *            / 360
 * Range:     u8 none, u16 n <= 3, u32 n <= 26, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t p = 474 + n_seq * (1247 + n_seq * (1215 + n_seq * (545 + n_seq * (111 + n_seq * 8))));
    return n_seq * p / 360;
}
