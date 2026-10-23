/*
 * A174033  T(n, m) = Sum_{i=0..10} floor(Eulerian(n+1, m)/2^i).
 * https://oeis.org/A174033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = T(r, m) where r = floor((sqrt(8*n+1)-1)/2), m = n -
 *            r*(r+1)/2, T(r, m) = Sum_{i=0..10} floor(Eulerian(r+1, m) / 2^i)
 *            The sequence is the triangle T(n,m) flattened row by row
 * Range:     u8 none, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>

/* -------------  Global state for memoisation  ----------------- */

static seq_t *e_prev = NULL;          /* Eulerian row n-1, length n-1 */
static size_t  e_prev_len = 0;        /* length of e_prev            */
static seq_t *tri = NULL;             /* flattened triangle values   */
static size_t  tri_len = 0;           /* number of values in tri     */
static idx_t   last_eulerian_n = 0;   /* last computed Eulerian n    */

/* ---------  Utility: find the row containing index n  --------- */
static idx_t row_of_idx(idx_t n)
{
    /* solve r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (idx_t)((sqrt((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);
    /* correct possible rounding errors */
    while ((idx_t)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((idx_t)r * (r + 1) / 2 > n) --r;
    return r;
}

/* -------------  Pre‑compute triangle up to given row ---------- */
static void ensure_rows(idx_t row_needed)
{
    /* we must compute Eulerian rows up to n = row_needed+1          */
    const idx_t target_n = row_needed + 1;
    while (last_eulerian_n < target_n) {
        /* compute next Eulerian row of length n = last_n+1          */
        const idx_t n = last_eulerian_n + 1;
        seq_t *cur = (seq_t *)malloc(n * sizeof(seq_t));
        if (!cur) abort();  /* out of memory */

        if (n == 1) {                  /* Eulerian(1,0) = 1 */
            cur[0] = (seq_t)1;
        } else {
            /* recurrence:  E(n,k) = (k+1)E(n-1,k) + (n-k)E(n-1,k-1) */
            for (idx_t k = 0; k < n; ++k) {
                seq_t term1 = 0, term2 = 0;
                if (k < e_prev_len) term1 = ((seq_t)(k + 1)) * e_prev[k];
                if (k > 0)          term2 = ((seq_t)(n - k))  * e_prev[k - 1];
                cur[k] = term1 + term2;
            }
        }

        /* append the row to the flattened triangle */
        seq_t *new_tri = (seq_t *)realloc(tri, (tri_len + n) * sizeof(seq_t));
        if (!new_tri) abort();
        tri = new_tri;
        for (idx_t k = 0; k < n; ++k) {
            /* compute T(n-1, k) = sum floor(cur[k] / 2^i)  (i=0..10) */
            seq_t val = 0;
            seq_t denom = 1;
            for (idx_t i = 0; i < 11; ++i) {
                val += cur[k] / denom;
                denom <<= 1;
            }
            tri[tri_len + k] = val;
        }
        tri_len += n;

        /* prepare for next iteration */
        free(e_prev);
        e_prev = cur;
        e_prev_len = n;
        ++last_eulerian_n;            /* now last_eulerian_n == n */
    }
}

/* -------------------------  Main function  -------------------- */
seq_t a(idx_t n)
{
    const idx_t r = row_of_idx(n);
    const idx_t offset = r * (r + 1) / 2;
    const idx_t m = n - offset;
    ensure_rows(r);
    return tri[offset + m];
}
