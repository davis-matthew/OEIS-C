/*
 * A018813  Number of lines through exactly 6 points of an n X n grid of
 *          points.
 * https://oeis.org/A018813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of straight lines that pass through exactly 6 points of
 *            an n×n grid of integer lattice points. The algorithm enumerates
 *            all primitive directions (dx,dy) with gcd(dx,|dy|)=1 and for
 *            each direction counts the base points that represent lines of
 *            length 6 with no extra lattice point on either end
 * Range:     u8 n <= 17, u16 n <= 82, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd64(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 6)
        return 0;

    seq_t res = 0;

    /* Vertical lines contain exactly 6 points only when n==6. */
    if (n == 6)
        res += (seq_t)n;   /* 6 vertical lines */

    idx_t maxdx = (n - 1u) / 5u;
    idx_t maxdy = (n - 1u) / 5u;

    for (idx_t dx = 1u; dx <= maxdx; ++dx) {
        for (int64_t dy = -(int64_t)maxdy; dy <= (int64_t)maxdy; ++dy) {
            /* skip non‑primitive slopes */
            if (dy == 0 && dx != 1) continue;     /* only dx==1 can give horizontal lines */
            idx_t ady = (dy < 0 ? (idx_t)(-dy) : (idx_t)dy);
            if (gcd64(dx, ady) != 1)
                continue;

            /* bounds for the first base point of the 6‑point segment */
            int64_t X1 = 0;
            int64_t X2 = (int64_t)n - 1 - 5 * (int64_t)dx;
            if (X2 < X1)
                continue;   /* no points for this direction */

            int64_t Y1, Y2;
            if (dy >= 0) {
                Y1 = 0;
                Y2 = (int64_t)n - 1 - 5 * (int64_t)dy;
            } else {
                Y1 = -5 * (int64_t)dy;           /* -5*dy positive */
                Y2 = (int64_t)n - 1;
            }
            if (Y2 < Y1)
                continue;   /* empty rectangle */

            int64_t total = (X2 - X1 + 1) * (Y2 - Y1 + 1);

            /* predecessor inside the grid */
            int64_t pred = 0;
            if (dy >= 0) {
                int64_t px1 = (int64_t)dx;
                int64_t px2 = X2;
                int64_t py1 = (int64_t)dy;
                int64_t py2 = Y2;
                if (px1 <= px2 && py1 <= py2)
                    pred = (px2 - px1 + 1) * (py2 - py1 + 1);
            } else {
                int64_t U = (int64_t)n - 1 + dy;   /* y0 - dy < 0  ->  y0 > n-1+dy */
                int64_t px1 = (int64_t)dx;
                int64_t px2 = X2;
                int64_t py1 = Y1;
                int64_t py2 = U;
                if (px1 <= px2 && py1 <= py2)
                    pred = (px2 - px1 + 1) * (py2 - py1 + 1);
            }

            /* successor inside the grid */
            int64_t succ = 0;
            if (dy >= 0) {
                int64_t sx1 = X1;
                int64_t sx2 = (int64_t)n - 1 - 6 * (int64_t)dx;
                if (sx2 > X2) sx2 = X2;          /* intersection with base rectangle */
                int64_t sy1 = Y1;
                int64_t sy2 = (int64_t)n - 1 - 6 * (int64_t)dy;
                if (sy2 > Y2) sy2 = Y2;
                if (sx1 <= sx2 && sy1 <= sy2)
                    succ = (sx2 - sx1 + 1) * (sy2 - sy1 + 1);
            } else {
                int64_t sx1 = X1;
                int64_t sx2 = (int64_t)n - 1 - 6 * (int64_t)dx;
                if (sx2 > X2) sx2 = X2;
                int64_t sy1 = -6 * (int64_t)dy;
                int64_t sy2 = Y2;
                if (sy1 < Y1) sy1 = Y1;
                if (sx1 <= sx2 && sy1 <= sy2)
                    succ = (sx2 - sx1 + 1) * (sy2 - sy1 + 1);
            }

            /* both predecessor and successor inside the grid */
            int64_t both = 0;
            int64_t bx1, bx2, by1, by2;
            if (dy >= 0) {
                bx1 = (int64_t)dx;
                bx2 = (int64_t)n - 1 - 6 * (int64_t)dx;
                if (bx2 > X2) bx2 = X2;
                by1 = (int64_t)dy;
                by2 = (int64_t)n - 1 - 6 * (int64_t)dy;
                if (by2 > Y2) by2 = Y2;
            } else {
                bx1 = (int64_t)dx;
                bx2 = (int64_t)n - 1 - 6 * (int64_t)dx;
                if (bx2 > X2) bx2 = X2;
                by1 = -6 * (int64_t)dy;
                by2 = (int64_t)n - 1 + dy;
                if (by1 < Y1) by1 = Y1;
            }
            if (bx1 <= bx2 && by1 <= by2)
                both = (bx2 - bx1 + 1) * (by2 - by1 + 1);

            int64_t add = total - pred - succ + both;
            if (add < 0)
                add = 0;

            res += (seq_t)add;
        }
    }

    return res;
}
