/*
 * A006422  Number of rooted toroidal maps with 2 faces and n vertices and
 *          without separating cycles or isthmuses.
 * https://oeis.org/A006422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(8*n^3 + 87*n^2 + 148*n - 3)/360
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t poly = 8*n_seq*n_seq*n_seq + 87*n_seq*n_seq + 148*n_seq - 3;
    seq_t result = n_seq * (n_seq + 1) * (n_seq + 2) * poly / 360;
    return result;
}
