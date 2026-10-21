/*
 * A063722  Number of edges in the Platonic solids (in the order tetrahedron,
 *          cube, octahedron, dodecahedron, icosahedron).
 * https://oeis.org/A063722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = 6,12,12,30,30 for n=1..5; a(n)=0 for n>5
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t edges[6] = {0, 6, 12, 12, 30, 30}; /* index 1..5 valid */
    if (n <= 5)
        return edges[n];
    return 0; /* undefined for n>5 */
}
