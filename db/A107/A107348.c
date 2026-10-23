/*
 * A107348  Triangle read by rows: T(m,n) = number of different lines in a
 *          rectangular m X n array of points with integer coordinates (x,y):
 *          0 <= x <= m, 0 <= y <= n.
 * https://oeis.org/A107348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) counts the number of distinct lines determined by the
 *            points of a rectangular grid of size (m+1) × (n+1), where m and
 *            n are obtained from the index n by reading the OEIS triangle
 *            row‑by‑row
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed (not required by the build) */

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute T(m,n) – number of distinct lines in an (m+1)×(n+1) lattice grid. */
static inline seq_t lines_in_grid(idx_t m, idx_t n)
{
    if (m == 0 && n == 0) return 0;          /* only one point – no line */
    seq_t total = 0;

    /* Vertical lines exist only if there are at least two distinct y‑coordinates */
    if (n >= 1) total += (seq_t)(m + 1);      /* x = const  lines */
    /* Horizontal lines exist only if there are at least two distinct x‑coordinates */
    if (m >= 1) total += (seq_t)(n + 1);      /* y = const  lines */

    if (m == 0 || n == 0) return total;      /* no further non‑horizontal/vertical lines */

    /* Count lines with non‑vertical, non‑horizontal slopes.
       For each coprime pair (dx,dy) we count the lines whose smallest
       member (x0,y0) satisfies:
           x0 + dx <= m ,  y0 + dy <= n
           x0 < dx  OR  y0 < dy                      (minimality)
       The contribution of such a (dx,dy) is:
           total_pairs - pairs_with_both_offsets >= dx,dy
       and each slope occurs twice (positive and negative). */
    for (idx_t dx = 1; dx <= m; ++dx) {
        for (idx_t dy = 1; dy <= n; ++dy) {
            if (gcd_idx(dx, dy) != 1) continue;

            idx_t a = m - dx + 1;
            idx_t b = n - dy + 1;
            seq_t total_pairs = (seq_t)a * (seq_t)b;

            idx_t c = 0;
            if (m > 0 && m >= 2 * dx) c = m - 2 * dx + 1;
            idx_t d = 0;
            if (n > 0 && n >= 2 * dy) d = n - 2 * dy + 1;
            seq_t sub = (seq_t)c * (seq_t)d;

            seq_t add = total_pairs > sub ? total_pairs - sub : 0;
            total += (seq_t)2 * add;   /* account for both positive and negative slopes */
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    /* Determine the row index m and column index k (n) from the flat index n.
       The triangular numbering has row lengths 1,2,3,… the cumulative counts
       are r(r+1)/2 for the start index of row r. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) ++m;  /* find row m  */
    idx_t k = n - m * (m + 1) / 2;           /* column index within the row */

    return lines_in_grid(m, k);
}
