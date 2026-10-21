/*
 * A063925  Number of 2-dimensional faces in the regular 4-dimensional
 *          polytopes.
 * https://oeis.org/A063925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = number of 2‑dimensional faces of the n‑th regular
 *            4‑dimensional polytope. The six regular 4‑polytopes (in order)
 *            are: 5‑cell, 8‑cell (hypercube), 16‑cell (cross‑polytope),
 *            24‑cell, 120‑cell, 600‑cell. The values are computed by simple
 *            combinatorial formulas for each case. For n>6 the funct...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n) {
        case 1: /* 5‑cell: 5 vertices → C(5,3) triangles = 10 */
            return (seq_t)10;
        case 2: /* 8‑cell (hypercube): C(4,2)·2² squares = 24 */
            return (seq_t)6 * (seq_t)4;
        case 3: /* 16‑cell (cross‑polytope) with 8 vertices:
                    total triples = C(8,3)=56,
                    triples containing an opposite pair = 4·6 = 24,
                    triangles = 56–24 = 32 */
            return (seq_t)56 - (seq_t)24;
        case 4: /* 24‑cell: 24 octahedral cells, each contributes 4 distinct
                    square faces (each square shared by 4 cells) → 24·4 = 96 */
            return (seq_t)24 * (seq_t)4;
        case 5: /* 120‑cell: 120 dodecahedral cells, each with 12 pentagons;
                    pentagons shared by 2 cells → 120·12/2 = 720 */
            return (seq_t)120 * (seq_t)12 / (seq_t)2;
        case 6: /* 600‑cell: 600 tetrahedral cells, each with 4 triangles;
                    triangles shared by 2 cells → 600·4/2 = 1200 */
            return (seq_t)600 * (seq_t)4 / (seq_t)2;
        default:
            return (seq_t)0;
    }
}
