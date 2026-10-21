/*
 * A092537  Sorted numbers of faces in the Archimedean polyhedra.
 * https://oeis.org/A092537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = the integer that is the n‑th smallest face count of an
 *            Archimedean polyhedron (Prisms and antiprisms excluded)
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is finite (13 terms). For n > 13 the sequence is undefined; the implementation returns 0. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 1‑indexed table of face counts in sorted order for the 13 Archimedean polyhedra. */
    static const seq_t faces[] = {
        /* dummy for 0 index */
        0,
        8,   /* truncated tetrahedron                       */
        14,  /* cuboctahedron/truncated cube/truncated octahedron   */
        14,  /* same again (duplicate counts are kept independently) */
        14,  /* duplicate for completeness */
        26,  /* great rhombicuboctahedron / small rhombicuboctahedron  */
        26,  /* duplicate */
        32,  /* icosidodecahedron / truncated dodecahedron / truncated icosahedron */
        32,  /* duplicate */
        32,  /* duplicate */
        38,  /* snub cube */
        62,  /* great rhombicosidodecahedron / small rhombicosidodecahedron */
        62,  /* duplicate */
        92   /* snub dodecahedron */
    };
    /* Table length including dummy */
    static const size_t len = sizeof(faces) / sizeof(faces[0]);

    /* Return the term if it exists, otherwise 0. */
    if (n >= 1 && (size_t)n < len)
        return faces[(size_t)n];
    else
        return (seq_t)0;
}
