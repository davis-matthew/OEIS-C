/*
 * A178258  Number of collinear point 6-tuples in an n X n cubical grid.
 * https://oeis.org/A178258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of collinear 6‑tuples in an n x n square lattice.
 *            The algorithm enumerates all primitive direction vectors, counts
 *            the number of distinct lines having at least 6 lattice points,
 *            and for each line adds C(L,6) where L is the number of points on
 *            that line
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 6) return 0;

    /* contribution from horizontal and vertical lines */
    seq_t lines_per_axis = binom(n, 6);   /* C(n,6) */
    seq_t total = lines_per_axis * (seq_t)(2 * n);  /* 2 axes, each with n lines */

    /* iterate over primitive direction vectors (dx>0, dy>0) */
    for (idx_t dx = 1; dx <= n - 1; ++dx) {
        /* if either direction cannot support 6 points reject early */
        if ((n - 1) / dx < 5) continue;          /* min((n-1)/dx,(n-1)/dy) < 5  */
        for (idx_t dy = 1; dy <= n - 1; ++dy) {
            if ((n - 1) / dy < 5) continue;      /* same check on dy   */
            if (gcd_idx(dx, dy) != 1) continue;  /* must be primitive */

            /* positive slope contribution */
            seq_t contrib = 0;
            idx_t Lmax = 1 + ( ( (n - 1) / dx < (n - 1) / dy )
                                ? (n - 1) / dx : (n - 1) / dy );

            for (idx_t L = 6; L <= Lmax; ++L) {
                int A  = (int)(n - (L - 1) * dx);
                int B  = (int)(n - (L - 1) * dy);
                if (A <= 0 || B <= 0) break;

                int A2 = (int)(n - L * dx);
                int B2 = (int)(n - L * dy);

                int x1 = A - (int)dx;
                if (x1 < 0) x1 = 0;
                int y1 = B - (int)dy;
                if (y1 < 0) y1 = 0;
                idx_t cnt_at_least_L = (idx_t)A * (idx_t)B - (idx_t)x1 * (idx_t)y1;

                idx_t cnt_at_least_L1 = 0;
                if (A2 > 0 && B2 > 0) {
                    int x2 = A2 - (int)dx;
                    if (x2 < 0) x2 = 0;
                    int y2 = B2 - (int)dy;
                    if (y2 < 0) y2 = 0;
                    cnt_at_least_L1 = (idx_t)A2 * (idx_t)B2 - (idx_t)x2 * (idx_t)y2;
                }

                idx_t lines_exact = cnt_at_least_L > cnt_at_least_L1 ?
                                    cnt_at_least_L - cnt_at_least_L1 : 0;
                if (!lines_exact) continue;

                seq_t ways = binom(L, 6);         /* C(L,6) */
                contrib += ways * (seq_t)lines_exact;
            }

            /* both positive and negative slopes */
            total += contrib * 2;
        }
    }
    return total;
}
