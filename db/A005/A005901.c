/*
 * A005901  Number of points on surface of cuboctahedron (or icosahedron):
 *          a(0) = 1; for n > 0, a(n) = 10n^2 + 2. Also coordination sequence
 *          for f.c.c. or A_3 or D_3 lattice.
 * https://oeis.org/A005901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = 10*n^2 + 2
 * Range:     u8 n <= 5, u16 n <= 80, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    seq_t n_seq = (seq_t)n;
    return 10 * n_seq * n_seq + 2;
}
