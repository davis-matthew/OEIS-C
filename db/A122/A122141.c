/*
 * A122141  Array: T(d,n) = number of ways of writing n as a sum of d squares,
 *          read by ascending antidiagonals.
 * https://oeis.org/A122141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of ordered d‑tuples of integers whose squares sum
 *            to m, where (d,m) are obtained by reading the table T(d,m) along
 *            ascending antidiagonals
 * Range:     u8 n <= 48, u16 n <= 177, u32 n <= 573, u64 n <= 2103,
 *            u128 n <= 7923, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Global cache for the DP table T(d,n).  */
static seq_t *table = NULL;          /* flattened table: d*(nmax+1)+n   */
static idx_t  stored_dmax = 0;       /* current max d stored          */
static idx_t  stored_nmax = 0;       /* current max n stored          */

/* Integer floor square root for idx_t values (small n, fast). */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t r = (idx_t)sqrt((double)v);
    while ((r + 1) * (r + 1) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

/* Rebuild the DP table up to given limits.  */
static void recompute_table(idx_t dmax, idx_t nmax)
{
    size_t stride = (size_t)(nmax + 1);
    size_t sz     = stride * (size_t)(dmax + 1);
    seq_t *new_tbl = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!new_tbl) return;    /* allocation failed – caller will tolerate */

    /* Base case: T(0,0) = 1 */
    new_tbl[0] = 1;

    for (idx_t d = 1; d <= dmax; ++d) {
        for (idx_t n = 0; n <= nmax; ++n) {
            seq_t val = new_tbl[(size_t)(d - 1) * stride + n];   /* k = 0 */
            idx_t m = isqrt_u64(n);
            for (idx_t t = 1; t <= m; ++t) {
                idx_t n_minus = n - t * t;
                val += (seq_t)2 * new_tbl[(size_t)(d - 1) * stride + n_minus];
            }
            new_tbl[(size_t)d * stride + n] = val;
        }
    }

    free(table);
    table = new_tbl;
    stored_dmax = dmax;
    stored_nmax = nmax;
}

/* OEIS A122141: antidiagonal enumeration of representations of n
 * as a sum of d squares.  */
seq_t a(idx_t n)
{
    /* Find the antidiagonal s such that s(s+1)/2 >= n.  */
    idx_t s = 1;
    while (s * (s + 1) / 2 < n) ++s;

    /* Offset within this diagonal (0‑based).  */
    idx_t prev    = s * (s - 1) / 2;
    idx_t offset  = n - prev - 1;     /* 0-based */
    idx_t d_val   = s - offset;
    idx_t n_val   = offset;

    /* Expand cache if necessary.  */
    if (d_val > stored_dmax || n_val > stored_nmax) {
        recompute_table(d_val, n_val);
    }

    size_t stride = (size_t)(stored_nmax + 1);
    return table[(size_t)d_val * stride + n_val];
}
