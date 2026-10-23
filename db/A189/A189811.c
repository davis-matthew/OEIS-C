/*
 * A189811  Number of right triangles on a (n+1)X7 grid.
 * https://oeis.org/A189811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of right triangles on an (n+1)×7 grid. * Computed
 *            by counting unordered pairs of perpendicular vectors from each
 *            grid point
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* offset starts at 1, value undefined below */

    /* Contribution from horizontal‑vertical vector pairs (only such pairs give a
     * right angle where one leg is horizontal and the other vertical).  For every
     * grid point there are n horizontal and 6 vertical vectors; hence 6*n pairs
     * per point.  Summing over all (n+1)×7 points gives 42 n (n+1). */
    seq_t result = (seq_t)42 * (seq_t)n * (seq_t)(n + 1);

    /* Count remaining pairs of vectors whose both vertical components are
     * non‑zero.  For a pair (dx1,dy1),(dx2,dy2) the dot product is zero if
     * dx1*dx2 + dy1*dy2 == 0.  Rearranged: dx2 = -dy1*dy2 / dx1.
     * Because |dy1*dy2| ≤ 36, for each xi the possible dx2 values are few,
     * so this loop is fast even for n≈10000. */
    int N = (int)n;   /* n is at most about 10000, fits in int */
    for (int dy1 = -6; dy1 <= 6; ++dy1) {
        if (dy1 == 0) continue;
        for (int dx1 = -N; dx1 <= N; ++dx1) {
            if (dx1 == 0) continue;
            for (int dy2 = -6; dy2 <= 6; ++dy2) {
                if (dy2 == 0) continue;

                int k = -dy1 * dy2;                /* dx1 * dx2 = k   */
                if (k % dx1 != 0) continue;
                int dx2 = k / dx1;                 /* integer division */
                if (dx2 == 0) continue;
                if (dx2 < -N || dx2 > N) continue;

                /* Enumerate each unordered pair only once.  Use a lexicographic
                 * ordering on (dy,dx). */
                if (!(dy1 < dy2 || (dy1 == dy2 && dx1 < dx2))) continue;

                /* Number of starting points P for which both vectors are
                 * admissible.  For the x‑coordinate Lx = #{ i | 0 ≤ i ≤ N,
                 * 0 ≤ i+dx1 ≤ N, 0 ≤ i+dx2 ≤ N };
                 * for the y‑coordinate Ly = #{ j | 0 ≤ j ≤ 6,
                 * 0 ≤ j+dy1 ≤ 6, 0 ≤ j+dy2 ≤ 6 }.
                 * These can be computed in O(1). */
                int xmax = (dx1 > dx2) ? dx1 : dx2;
                if (xmax < 0) xmax = 0;
                int xmin = (dx1 < dx2) ? dx1 : dx2;
                int low_x  = (xmin < 0) ? -xmin : 0;
                int high_x = N - xmax;
                int Lx = (high_x >= low_x) ? (high_x - low_x + 1) : 0;

                int ymax = (dy1 > dy2) ? dy1 : dy2;
                if (ymax < 0) ymax = 0;
                int ymin = (dy1 < dy2) ? dy1 : dy2;
                int low_y  = (ymin < 0) ? -ymin : 0;
                int high_y = 6 - ymax;
                int Ly = (high_y >= low_y) ? (high_y - low_y + 1) : 0;

                if (Lx == 0 || Ly == 0) continue;
                result += (seq_t)Lx * (seq_t)Ly;
            }
        }
    }
    return result;
}
