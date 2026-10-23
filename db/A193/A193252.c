/*
 * A193252  Great rhombicuboctahedron with faces of centered polygons.
 * https://oeis.org/A193252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n^3 - 36*n^2 + 14*n - 1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 564, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (seq_t)24 * nn * nn * nn
         - (seq_t)36 * nn * nn
         + (seq_t)14 * nn
         - (seq_t)1;
}
