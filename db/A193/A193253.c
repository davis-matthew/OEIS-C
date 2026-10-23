/*
 * A193253  Great rhombicosidodecahedron with faces of centered polygons.
 * https://oeis.org/A193253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60*n^3 - 90*n^2 + 32*n - 1
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 415, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t N2 = N * N;        /* n^2 */
    seq_t N3 = N2 * N;       /* n^3 */
    /* Compute the polynomial, all operations in seq_t */
    seq_t res = (seq_t)60 * N3 - (seq_t)90 * N2 + (seq_t)32 * N - (seq_t)1;
    return res;
}
