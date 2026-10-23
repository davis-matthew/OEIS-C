/*
 * A178106  Number of closed walks of length 16 and algebraic area n in the
 *          square lattice.
 * https://oeis.org/A178106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of closed walks of length 16 on the square lattice
 *            with algebraic area n (signed), n >= 0
 * Range:     u8 none, u16 none, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int dx[4] = {0, 1, 0, -1};
static const int dy[4] = {1, 0, -1, 0};

seq_t a(idx_t n)
{
    static int ready = 0;
    static seq_t seq[17];

    if (!ready) {
        /* half‑walk statistics: endpoint (dx,dy) ∈ [-8,8]^2
           and signed area2 (twice the algebraic area) ∈ [-16,16]      */
        static seq_t half_counts[17][17][33];

        /* total area distribution for closed walks of length 16 */
        static seq_t area_counts[65];             /* index = area_total + 32 */

        /* enumerate all length‑8 walks (4^8 = 65536) */
        const uint32_t max_mask = 1U << 16;       /* 2 bits per step */
        for (uint32_t mask = 0; mask < max_mask; ++mask) {
            int x = 0, y = 0, area2 = 0;
            for (int step = 0; step < 8; ++step) {
                int dir = (mask >> (2 * step)) & 3;
                int nx = x + dx[dir];
                int ny = y + dy[dir];
                area2 += x * ny - nx * y;   /* cross product contribution */
                x = nx;
                y = ny;
            }
            int ix = x + 8;
            int iy = y + 8;
            int ia = area2 + 16;
            half_counts[ix][iy][ia] += (seq_t)1;
        }

        /* combine two half‑walks that end at the same point */
        for (int ix = 0; ix < 17; ++ix) {
            for (int iy = 0; iy < 17; ++iy) {
                /* skip empty groups for speed */
                int i, j, k, w;
                int any = 0;
                for (i = 0; i < 33 && !any; ++i)
                    for (j = 0; j < 33; ++j)
                        if (half_counts[ix][iy][i])
                            any = 1;
                if (!any) continue;

                for (i = 0; i < 33; ++i) {
                    seq_t c1 = half_counts[ix][iy][i];
                    if (!c1) continue;
                    int area1 = i - 16;
                    for (j = 0; j < 33; ++j) {
                        seq_t c2 = half_counts[ix][iy][j];
                        if (!c2) continue;
                        int area2 = j - 16;
                        int total = area1 - area2; /* signed area of full walk */
                        int idx = total + 32;      /* shift to non‑negative */
                        area_counts[idx] += c1 * c2;
                    }
                }
            }
        }

        /* build the final sequence for n = 0..16 (area = n) */
        for (int i = 0; i <= 16; ++i) {
            int area2 = 2 * i;          /* area2 = 2 * area */
            int idx = area2 + 32;        /* shift */
            seq[i] = area_counts[idx];
        }

        ready = 1;
    }

    if (n <= 16)
        return seq[(int)n];
    else
        return (seq_t)0;
}
