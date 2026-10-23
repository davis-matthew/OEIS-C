/*
 * A192839  Molecular topological indices of the square graphs.
 * https://oeis.org/A192839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n*(n+1)*(n-1)^3
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 56, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = n;
    seq_t nm1 = sn - 1;
    seq_t nm1_2 = nm1 * nm1;
    seq_t nm1_3 = nm1_2 * nm1;
    seq_t sn1 = sn + 1;
    return (seq_t)8 * sn * sn1 * nm1_3;
}
