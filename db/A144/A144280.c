/*
 * A144280  Lower triangular array called S2hat(-3) related to partition
 *          number array A144279.
 * https://oeis.org/A144280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the nth term of the lower‑triangular array S2hat(-3).
 *            The array entry at row r and column m (1≤m≤r) equals the sum
 *            over all partitions of r into m parts of the product of the
 *            4‑factorial numbers for each part size. a(n) is obtained by
 *            reading the array row‑wise
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 45, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;      /* flat 2‑D array for a(r,m) values          */
static size_t dp_rows = 0;    /* current maximum row handled by dp          */
static seq_t *w  = NULL;     /* weight array: w[s] = product_{k=1}^{s-1}(4k-1)    */
static idx_t max_w = 0;       /* maximal s for which w[s] is defined          */

/* recompute dp up to new_rmax (inclusive).  All entries for rows ≤ new_rmax
   are filled; the array is resized as needed. */
static void extend_to(idx_t new_rmax)
{
    if (new_rmax <= dp_rows) return;

    size_t width = (size_t)new_rmax + 1;
    size_t new_size = width * width;

    seq_t *newdp = (seq_t*)calloc(new_size, sizeof(seq_t));
    if (!newdp) return;    /* allocation failed – caller will get 0 */

    /* ensure weight array covers all sizes up to new_rmax */
    if (new_rmax > max_w) {
        seq_t *neww = (seq_t*)realloc(w, (size_t)(new_rmax + 1) * sizeof(seq_t));
        if (!neww) { free(newdp); return; }
        w = neww;
        if (max_w < 1) w[1] = (seq_t)1;
        for (idx_t s = max_w + 1; s <= new_rmax; ++s) {
            if (s == 1) w[s] = (seq_t)1;
            else w[s] = w[s-1] * ((seq_t)4*s - 5);
        }
        max_w = new_rmax;
    }

    /* base case: one way to partition 0 into 0 parts. */
    newdp[0] = (seq_t)1;

    /* coin‑change DP: for each part size s we add unlimited copies. */
    for (idx_t s = 1; s <= new_rmax; ++s) {
        seq_t ws = w[s];
        for (idx_t i = s; i <= new_rmax; ++i) {
            size_t base_i      = (size_t)i      * width;
            size_t base_i_s    = (size_t)(i-s)   * width;
            for (idx_t j = 1; j <= i; ++j) {
                /* newdp[i][j] += newdp[i-s][j-1] * w[s] */
                newdp[base_i + j] += newdp[base_i_s + j - 1] * ws;
            }
        }
    }

    free(dp);
    dp = newdp;
    dp_rows = new_rmax;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* find row r such that T(r-1) < n <= T(r), where T(r)=r*(r+1)/2 */
    idx_t r = 1;
    while (((idx_t)r * (r + 1)) / 2 < n) ++r;

    idx_t prev_tri = ((idx_t)(r - 1) * r) / 2;
    idx_t c = n - prev_tri;   /* column (1‑based) */

    extend_to(r);             /* ensure dp has all requested rows */

    size_t width = (size_t)dp_rows + 1;
    return dp[(size_t)r * width + c];
}
