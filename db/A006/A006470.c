/*
 * A006470  Number of tree-rooted planar maps with 3 faces and n vertices and
 *          no isthmuses.
 * https://oeis.org/A006470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(n+2)*(n+3)/24
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n1 = n_seq + 1;
    seq_t n2 = n_seq + 2;
    seq_t n3 = n_seq + 3;
    
    seq_t result = n_seq * n1 / 2;
    result = result * n1 * n2 * n3 / 12;
    
    return result;
}
