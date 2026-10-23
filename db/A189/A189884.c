/*
 * A189884  Number of 1:4:sqrt(17) proportioned triangles on a (n+1) X (n+1)
 *          grid.
 * https://oeis.org/A189884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 115, u64 n <= 115,
 *            u128 n <= 115, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the OEIS A189884: Number of 1:4:sqrt(17) proportioned triangles
   on an (n+1)×(n+1) grid.
   The method enumerates every possible short leg vector u=(a,b) with
   integer coordinates such that |a|,|b| ≤ n/4.  For each u the two
   corresponding long legs are v₁=(-4b,4a) and v₂=(4b,-4a).  Each
   ordered pair (u,vₖ) has positions for the right‑angled vertex
   P counted by intersecting the constraints
      0 ≤ P.x ≤ n , 0 ≤ P.y ≤ n
      0 ≤ P.x+a ≤ n , 0 ≤ P.y+b ≤ n
      0 ≤ P.x+vₖ.x ≤ n , 0 ≤ P.y+vₖ.y ≤ n.
   The number of valid P is the product of the lengths of the allowed
   x‑ and y‑intervals.  Summing over all u and both v directions
   yields the required count.  This enumeration is O(n²) and fast
   enough for n up to about 10⁴. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    /* floor(n/4) bounds for the short leg components */
    int64_t N4 = (int64_t)n / 4;
    seq_t total = 0;

    for (int64_t a = -N4; a <= N4; ++a) {
        for (int64_t b = -N4; b <= N4; ++b) {
            if (a == 0 && b == 0) continue; /* zero short leg */
            /* two choices of the long leg */
            int64_t vx[2] = { -4 * b,  4 * b }; /* v₁.x, v₂.x */
            int64_t vy[2] = {  4 * a, -4 * a }; /* v₁.y, v₂.y */

            for (int k = 0; k < 2; ++k) {
                int64_t vxi = vx[k];
                int64_t vyi = vy[k];

                /* compute x‑bounds */
                int64_t x_min = 0;
                int64_t x_max = (int64_t)n;
                if (-a > x_min) x_min = -a;
                if (n - a < x_max) x_max = n - a;
                if (-vxi > x_min) x_min = -vxi;
                if (n - vxi < x_max) x_max = n - vxi;

                /* compute y‑bounds */
                int64_t y_min = 0;
                int64_t y_max = (int64_t)n;
                if (-b > y_min) y_min = -b;
                if (n - b < y_max) y_max = n - b;
                if (-vyi > y_min) y_min = -vyi;
                if (n - vyi < y_max) y_max = n - vyi;

                if (x_min <= x_max && y_min <= y_max) {
                    seq_t places =
                        (seq_t)(x_max - x_min + 1) *
                        (seq_t)(y_max - y_min + 1);
                    total += places;
                }
            }
        }
    }
    return total;
}
