/*
 * A116155  Triangle T(n,k) defined by: T(0,0)=1, T(n,k)=0 if k < 0 or k > n,
 *          T(n,k) = T(n-1,k-1) + k*T(n-1,k) + Sum_{j>=1} T(n-1,k+j).
 * https://oeis.org/A116155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(r,c) of triangle T(n,k) in row‑major order,
 *            where T is defined by T(0,0) = 1; T(n,k) = 0 if k < 0 or k > n;
 *            T(n,k) = T(n-1,k-1) + k*T(n-1,k) + Σ_{j≥1} T(n-1,k+j). The
 *            function builds rows of the triangle lazily and caches them
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 135, u64 n <= 152,
 *            u128 n <= 152, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **tri_rows = NULL; /* pointers to each triangle row */
static size_t  rows_cap = 0;     /* number of rows currently allocated */

/* Ensure rows 0 … target are computed and cached. */
static void ensure_rows(idx_t target)
{
    /* If the requested row is already present, nothing to do. */
    if ((size_t)target < rows_cap)
        return;

    /* Extend the storage for row pointers. */
    size_t new_cap = (size_t)target + 1;
    seq_t **tmp = (seq_t **)realloc(tri_rows, new_cap * sizeof(seq_t *));
    tri_rows = tmp;

    /* Allocate new rows, initialised to zero. */
    for (size_t i = rows_cap; i < new_cap; ++i)
        tri_rows[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));

    /* The very first row (row 0) has one entry T(0,0)=1. */
    if (new_cap > 0 && tri_rows[0])
        tri_rows[0][0] = (seq_t)1;

    /* Build any missing rows beyond the previously cached range. */
    for (size_t n = rows_cap; n < new_cap; ++n) {
        if (n == 0) /* row 0 already initialised */ continue;

        size_t prev_len = n;        /* length of row n‑1 */
        seq_t *prev      = tri_rows[n - 1];

        /* Compute suffix sums of the previous row:
           suff[j] = Σ_{t=j}^{prev_len-1} prev[t]  (suff[prev_len] = 0) */
        seq_t *suff = (seq_t *)malloc((prev_len + 1) * sizeof(seq_t));
        suff[prev_len] = (seq_t)0;
        for (size_t j = prev_len; j-- > 0; )
            suff[j] = suff[j + 1] + prev[j];

        seq_t *cur = tri_rows[n];
        for (size_t k = 0; k <= n; ++k) {
            seq_t val = (seq_t)0;

            /* T(n‑1, k‑1) */
            if (k > 0)
                val += prev[k - 1];

            /* k * T(n‑1, k) */
            if (k < prev_len)
                val += (seq_t)k * prev[k];

            /* Σ_{j≥1} T(n‑1, k+j) = suffix sum starting at k+1 */
            if (k + 1 < prev_len)
                val += suff[k + 1];

            cur[k] = val;
        }

        free(suff);
    }

    rows_cap = new_cap; /* update cached rows count */
}

/* Return the n‑th term of A116155. */
seq_t a(idx_t n)
{
    /* Determine the row `r` and column `c` such that a(n)=T(r,c). */
    idx_t r = 0;
    idx_t accum = 0;
    while (accum + r + 1 <= n) {
        accum += r + 1;
        r++;
    }
    idx_t c = n - accum;

    /* Ensure the needed row is computed. */
    ensure_rows(r);

    return tri_rows[(size_t)r][(size_t)c];
}
