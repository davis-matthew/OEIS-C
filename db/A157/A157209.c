/*
 * A157209  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) + m*f(n,k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1, f(n, k) = k if k <= floor(n/2) otherwise n-k, and m =
 *          3, read by rows.
 * https://oeis.org/A157209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle T(n, k, 3) read by rows. T(n, k, 3) =
 *            (3*(n-k)+1)*T(n-1, k-1, 3) + (3*k+1)*T(n-1, k, 3) + 3*f(n,
 *            k)*T(n-2, k-1, 3) with T(n, 0, 3) = T(n, n, 3) = 1, f(n, k) = k
 *            if k <= floor(n/2) else n-k
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 57, u64 n <= 141,
 *            u128 n <= 387, big any n
 * Helpers:   seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_one.h"   /* provides seq_one(void) -> seq_t */

/* cache of rows of the triangle */
static seq_t **g_tri = NULL;        /* array of pointers to rows */
static size_t  g_tri_rows = 0;      /* number of rows currently allocated (0 means none) */

/* ensure that rows 0 .. row are computed */
static void ensure_row(idx_t row)
{
    const idx_t m = 3;            /* fixed parameter */

    if (row < g_tri_rows)            /* already computed */
        return;

    /* extend the array of row pointers */
    g_tri = (seq_t**)realloc(g_tri, (row + 1) * sizeof(seq_t*));

    /* compute any new rows */
    for (idx_t r = g_tri_rows; r <= row; ++r)
    {
        size_t len = (size_t)(r + 1);

        /* allocate the row */
        g_tri[r] = (seq_t*)malloc(len * sizeof(seq_t));

        /* boundary values */
        g_tri[r][0]        = seq_one();
        g_tri[r][len - 1]  = seq_one();

        /* interior values */
        for (idx_t k = 1; k < r; ++k)
        {
            /* compute f(r, k) */
            idx_t f = (k <= r / 2) ? k : (r - k);

            /* (3*(r-k)+1) * T(r-1, k-1) */
            seq_t term1 = (seq_t)(m * (r - k) + 1) * g_tri[r-1][k-1];

            /* (3*k+1) * T(r-1, k) */
            seq_t term2 = (seq_t)(m * k + 1) * g_tri[r-1][k];

            /* 3*f * T(r-2, k-1)  (only if r >= 2) */
            seq_t term3 = 0;
            if (r >= 2)
                term3 = (seq_t)(m * f) * g_tri[r-2][k-1];

            g_tri[r][k] = term1 + term2 + term3;
        }
    }

    g_tri_rows = (size_t)(row + 1);   /* number of rows now allocated */
}

seq_t a(idx_t n)
{
    /* find the row that contains index n */
    idx_t idxStart = 0;   /* first index of the current row */
    idx_t row = 0;
    while (idxStart + (row + 1) <= n)
    {
        idxStart += (row + 1);
        ++row;
    }

    /* ensure the required row is computed */
    ensure_row(row);

    /* column within the row */
    idx_t k = n - idxStart;

    return g_tri[row][k];
}
