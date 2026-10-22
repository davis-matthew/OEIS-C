/*
 * A005899  Number of points on surface of octahedron; also coordination
 *          sequence for cubic lattice: a(0) = 1; for n > 0, a(n) = 4n^2 + 2.
 * https://oeis.org/A005899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = 4*n^2 + 2
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    seq_t m = (seq_t)n;
    return 4 * m * m + 2;
}
