/*
 * A053016  Numbers of vertices of Platonic solids in the order tetrahedron,
 *          octahedron, cube, icosahedron, dodecahedron.
 * https://oeis.org/A053016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 4,6,8,12,20 for n = 1..5; 0 otherwise. */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return (seq_t)4;
        case 2: return (seq_t)6;
        case 3: return (seq_t)8;
        case 4: return (seq_t)12;
        case 5: return (seq_t)20;
        default: return (seq_t)0;
    }
}
