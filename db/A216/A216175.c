/*
 * A216175  Number of all polyhedra (tetrahedra of any orientation and
 *          octahedra) of any size, formed when intersecting a regular
 *          tetrahedron by planes parallel to its sides and dividing its edges
 *          into n equal parts.
 * https://oeis.org/A216175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/288)*(7+9*(-1)^n-16*(-1)^(n mod
 *            3)+24*n+124*n^2+104*n^3+22*n^4)
 * Range:     u8 n <= 6, u16 n <= 29, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  Use int64_t for intermediate arithmetic.  */
    int64_t N = (int64_t)n;

    int64_t alt2 = (N & 1) == 0 ? 1 : -1;          /* (-1)^n   */
    int64_t alt3 = (N % 3 == 1) ? -1 : 1;          /* (-1)^(n mod 3) */

    int64_t num = 7;
    num += 9       * alt2;
    num -= 16      * alt3;
    num += 24      * N;
    num += 124     * (__int128)N * N;
    num += 104     * (__int128)N * N * N;
    num += 22      * (__int128)N * N * N * N;

    return (seq_t)(num / 288);
}
