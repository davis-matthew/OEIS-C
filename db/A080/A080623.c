/*
 * A080623  Consider 3 X 3 X 3 Rubik cube, but only allow the double slice
 *          group to act; sequence gives number of positions that are exactly
 *          n moves from the start, up to equivalence under the full group of
 *          order 48 of the cube.
 * https://oeis.org/A080623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1,2,4,15,25,3 for n = 0…5; 0 otherwise. * The
 *            double‑slice group of the 3×3×3 cube contains only 50
 *            inequivalent * positions. Their distance distribution from the
 *            solved state is * exactly [1,2,4,15,25,3], after which no
 *            positions remain reachable. * Hence a(n) = 0 for all n ≥ 6
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 4;
    if (n == 3) return 15;
    if (n == 4) return 25;
    if (n == 5) return 3;
    return 0;
}
