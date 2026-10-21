/*
 * A063723  Number of vertices in the Platonic solids (in the order
 *          tetrahedron, cube, octahedron, dodecahedron, icosahedron).
 * https://oeis.org/A063723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = number of vertices of the n-th Platonic solid in the
 *            fixed order (tetrahedron, cube, octahedron, dodecahedron,
 *            icosahedron)
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Offset is 1 */
    if (n < 1) {
        return (seq_t)0;
    }

    switch ((idx_t)(n-1)) {
        case 0: return (seq_t)4;   /* tetrahedron   */
        case 1: return (seq_t)8;   /* cube          */
        case 2: return (seq_t)6;   /* octahedron    */
        case 3: return (seq_t)20;  /* dodecahedron  */
        case 4: return (seq_t)12;  /* icosahedron   */
        default: return (seq_t)0;  /* undefined for n>5 */
    }
}
