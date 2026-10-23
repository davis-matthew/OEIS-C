/*
 * A193249  Snub dodecahedron with faces of centered polygons.
 * https://oeis.org/A193249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 50*n^3-75*n^2+27*n-1 = (2*n-1)*(25*n^2-25*n+1)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 441, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; 0 not defined */
    seq_t nn   = (seq_t)n;          /* cast index to seq_t for exact arithmetic */
    seq_t part1 = 2 * nn - 1;       /* (2*n - 1) */
    seq_t part2 = 25 * nn * nn - 25 * nn + 1;   /* (25*n^2 - 25*n + 1) */
    return part1 * part2;           /* multiply the two parts */
}
