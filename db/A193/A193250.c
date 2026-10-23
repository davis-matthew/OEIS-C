/*
 * A193250  Small rhombicuboctahedron with faces of centered polygons.
 * https://oeis.org/A193250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^3 - 24*n^2 + 10*n - 1
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 645, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 16 * nn * nn * nn;   /* 16*n^3   */
    val += 10 * nn;                  /* +10*n    */
    val -= 24 * nn * nn;             /* -24*n^2  */
    val -= 1;                        /* -1       */
    return val;
}
