/*
 * A211638  Number of ordered triples (w, x, y) with all terms in {1, ..., n}
 *          and w^2 + x^2 + y^2 < n.
 * https://oeis.org/A211638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #ordered triples (w,x,y) with 1≤w,x,y≤n and w^2 + x^2 +
 *            y^2 < n. Computation: iterate over w and x up to √n (since
 *            larger values cannot satisfy the inequality). For each pair
 *            (w,x) compute remaining space for y^2 and add the
 *            integer‑square‑root count. Complexity O(√n²) ~= O(n)
 * Range:     u8 n <= 62, u16 n <= 2578, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t count = 0;

    /* w must satisfy w^2 < n, hence w ≤ floor(√(n-1)) */
    idx_t wmax = (idx_t)floor_sqrt((seq_t)n - 1);

    for (idx_t w = 1; w <= wmax; ++w) {
        seq_t w2 = (seq_t)w * (seq_t)w;

        /* x must satisfy w^2 + x^2 < n, using the smallest possible y (y=1) */
        idx_t xmax = (idx_t)floor_sqrt((seq_t)n - w2 - 1);
        for (idx_t x = 1; x <= xmax; ++x) {
            seq_t x2 = (seq_t)x * (seq_t)x;
            seq_t t  = (seq_t)n - w2 - x2;   /* remaining allowance for y^2 */

            if (t <= 1) continue;           /* no y can satisfy y^2 < t */

            seq_t ymax = floor_sqrt(t - 1);  /* maximum y such that y^2 < t */
            if (ymax > (seq_t)n) ymax = (seq_t)n;  /* y must also be ≤ n */
            count += ymax;
        }
    }
    return count;
}
