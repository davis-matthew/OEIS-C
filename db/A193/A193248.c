/*
 * A193248  Truncated dodecahedron, and truncated icosahedron with faces of
 *          centered polygons.
 * https://oeis.org/A193248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*n^3 - 45*n^2 + 17*n - 1
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 523, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                        /* convert index to seq_t */
    seq_t term1 = (seq_t)30 * nn * nn * nn + (seq_t)17 * nn;
    seq_t term2 = (seq_t)45 * nn * nn + (seq_t)1;
    return term1 - term2;                       /* final result */
}
