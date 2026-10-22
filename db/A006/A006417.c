/*
 * A006417  Number of loopless rooted planar maps with 4 faces and n vertices
 *          and no isthmuses.
 * https://oeis.org/A006417
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2160 + 846*n - 1027*n^2 - 345*n^3 + 125*n^4 + 39*n^5 +
 *            2*n^6)/360
 * Range:     u8 none, u16 n <= 4, u32 n <= 33, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t n2 = n_val * n_val;
    seq_t n3 = n2 * n_val;
    seq_t n4 = n3 * n_val;
    seq_t n5 = n4 * n_val;
    seq_t n6 = n5 * n_val;
    
    seq_t numerator = 2160 + 846 * n_val - 1027 * n2 - 345 * n3 + 125 * n4 + 39 * n5 + 2 * n6;
    return numerator / 360;
}
