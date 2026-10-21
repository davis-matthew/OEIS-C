/*
 * A092536  Sorted numbers of edges in the Archimedean polyhedra.
 * https://oeis.org/A092536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Explicit list of the 13 edge counts of Archimedean solids (including duplicates). */
/* a(1)=18, a(2)=24, a(3)=36, a(4)=36, a(5)=48, a(6)=60, a(7)=60, a(8)=72, */
/* a(9)=90, a(10)=90, a(11)=120, a(12)=150, a(13)=180 */
/* For n<1 or n>13 the function returns 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t edges[13] = {
        (seq_t)18,  /* Truncated tetrahedron                */
        (seq_t)24,  /* Cuboctahedron                       */
        (seq_t)36,  /* Truncated cube                      */
        (seq_t)36,  /* Truncated octahedron                */
        (seq_t)48,  /* Small rhombicuboctahedron           */
        (seq_t)60,  /* Icosa­di­odecahedron                 */
        (seq_t)60,  /* Snub cube                           */
        (seq_t)72,  /* Great rhombicuboctahedron           */
        (seq_t)90,  /* Truncated dodecahedron              */
        (seq_t)90,  /* Truncated icosahedron               */
        (seq_t)120, /* Small rhombicosidodecahedron        */
        (seq_t)150, /* Snub dodecahedron                   */
        (seq_t)180  /* Great rhombicosidodecahedron        */
    };

    if (n < 1 || n > 13)
        return 0;
    return edges[(size_t)(n - 1)];
}
