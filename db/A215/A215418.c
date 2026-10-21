/*
 * A215418  Number of Regular and Stellar polytopes in n-dimensional Euclidean
 *          space, or -1 if infinite.
 * https://oeis.org/A215418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1, a(2)=-1, a(3)=9, a(4)=16, a(n)=3 for n>=5
 * Range:     i8 n <= 66, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
    case 1:
        return (seq_t)1;
    case 2:
        return (seq_t)-1;        /* infinite number of regular polygons */
    case 3:
        return (seq_t)9;         /* 5 convex + 4 regular star polyhedra */
    case 4:
        return (seq_t)16;        /* 4‑dimensional regular and stellar polytopes */
    default:
        return (seq_t)3;         /* simplex, cross‑polytope, hypercube for n≥5 */
    }
}
