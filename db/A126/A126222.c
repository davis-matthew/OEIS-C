/*
 * A126222  Triangle read by rows: T(n,k) is the number of 2-Motzkin paths
 *          (i.e., Motzkin paths with blue and red level steps) without red
 *          level steps on the x-axis, having length n and k level steps (0 <=
 *          k <= n).
 * https://oeis.org/A126222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of 2‑Motzkin paths of length n that have no red
 *            level step on the x‑axis, flattened triangle order
 * Range:     u8 n <= 37, u16 n <= 83, u32 n <= 239, u64 n <= 756,
 *            u128 n <= 2586, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

#define TRI_MAX 200          /* enough for n up to 10 000 */

static seq_t triangle[TRI_MAX + 1][TRI_MAX + 1] = {{ 1 }};  /* triangle[0][0] = 1 */
static idx_t max_computed = 0;  /* highest row that has been filled */

/* Compute the triangle up to row 'up_to' (inclusive).  Always recomputes
   from row 1 to up_to; row 0 is preset above. */
static void compute_triangle(idx_t up_to)
{
    if (up_to <= max_computed)
        return;

    /* DP arrays: f[h][k] – number of partial paths of current length
       with height h and total k level steps.  g is the next length. */
    static seq_t f[TRI_MAX + 1][TRI_MAX + 1];
    static seq_t g[TRI_MAX + 1][TRI_MAX + 1];

    /* Initialise for length 0 */
    memset(f, 0, sizeof(f));
    f[0][0] = 1;           /* one empty path */

    /* Build rows 1 .. up_to */
    for (idx_t len = 1; len <= up_to; ++len) {
        /* zero g for the next length */
        for (idx_t h = 0; h <= len; ++h)
            memset(g[h], 0, (len + 1) * sizeof(seq_t));

        /* transitions from f to g */
        for (idx_t h = 0; h <= len - 1; ++h) {
            for (idx_t k = 0; k <= len - 1; ++k) {
                seq_t val = f[h][k];
                if (!val)
                    continue;

                /* Up step */
                g[h + 1][k] += val;

                /* Down step – only if we are above the axis */
                if (h > 0)
                    g[h - 1][k] += val;

                /* Blue level step – always allowed */
                g[h][k + 1] += val;

                /* Red level step – only above the axis */
                if (h > 0)
                    g[h][k + 1] += val;
            }
        }

        /* copy g back to f for next iteration */
        for (idx_t h = 0; h <= len; ++h)
            memcpy(f[h], g[h], (len + 1) * sizeof(seq_t));

        /* store the row: paths that end at height 0 */
        for (idx_t k = 0; k <= len; ++k)
            triangle[len][k] = f[0][k];
    }

    max_computed = up_to;
}

/* Return the nth term of the flattened triangle. */
seq_t a(idx_t n)
{
    /* Determine row r and column k from the flattened index. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    if (r > max_computed)
        compute_triangle(r);

    return triangle[r][k];
}
