/*
 * A193251  Small rhombicosidodecahedron with faces of centered polygons.
 * https://oeis.org/A193251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 40*n^3 - 60*n^2 + 22*n - 1
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 475, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not part of the offset, but safe */
    seq_t nn = (seq_t)n;                  /* n as seq_t */
    seq_t n2 = nn * nn;                  /* n^2 */
    seq_t n3 = n2 * nn;                  /* n^3 */
    seq_t r = n3 * (seq_t)40;            /* 40*n^3 */
    r -= n2 * (seq_t)60;                 /* - 60*n^2 */
    r += nn * (seq_t)22;                 /* + 22*n */
    r -= (seq_t)1;                       /* - 1 */
    return r;
}
