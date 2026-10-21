/*
 * A059450  Triangle read by rows: T(n,k) = Sum_{j=0..k-1} T(n,j) +
 *          Sum_{j=1..n-k} T(n-j,k), with T(0,0)=1 and T(n,k) = 0 for k > n.
 * https://oeis.org/A059450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = ∑_{j=0}^{k-1} T(n,j) + ∑_{j=1}^{n-k} T(n-j,k),
 *            T(0,0)=1, T(n,k)=0 for k>n. a(n) is the triangle read by rows
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 102, u64 n <= 298,
 *            u128 n <= 1032, big any n
 * Helpers:   tri_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/tri_index.h"

static seq_t *tri = NULL;          /* flat storage for T values */
static seq_t *colCum = NULL;       /* cumulative sums for each column */
static size_t tri_cap = 0;         /* capacity of tri */
static size_t col_cap = 0;         /* capacity of colCum */
static size_t cur_row = 0;         /* highest row already computed */

/* Ensure that rows up to 'need' are computed and stored */
static void ensure_rows(idx_t need)
{
    if ((size_t)need <= cur_row)
        return;

    /* allocate/resize triangle array if necessary */
    size_t new_tri_cap = (size_t)(((__int128)need + 1) * (need + 2) / 2);
    if (new_tri_cap > tri_cap) {
        seq_t *tmp = (seq_t*)realloc(tri, new_tri_cap * sizeof(seq_t));
        if (!tmp) abort();
        tri = tmp;
        tri_cap = new_tri_cap;
    }

    /* allocate/resize column cumulative array if necessary */
    if ((size_t)need + 1 > col_cap) {
        seq_t *tmp = (seq_t*)realloc(colCum, (size_t)(need + 1) * sizeof(seq_t));
        if (!tmp) abort();
        for (size_t i = col_cap; i <= (size_t)need; ++i)
            tmp[i] = 0;
        /* Initialize the first column with T(0,0)=1 when first allocating */
        if (cur_row == 0 && col_cap < 1)
            tmp[0] = 1;
        colCum = tmp;
        col_cap = (size_t)need + 1;
    }

    /* compute new rows */
    for (size_t n = cur_row + 1; n <= (size_t)need; ++n) {
        seq_t rowSum = 0;
        for (size_t k = 0; k <= n; ++k) {
            size_t idx = tri_index((idx_t)n, (idx_t)k);
            seq_t val = rowSum + colCum[k];
            tri[idx] = val;
            rowSum += val;
            colCum[k] += val;
        }
    }
    cur_row = (size_t)need;
}

/* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    while (((__int128)r * (r + 1)) / 2 > n) r--;
    while (((__int128)(r + 1) * (r + 2)) / 2 <= n) r++;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;  /* T(0,0) */
    idx_t row = find_row(n);
    idx_t col = n - (idx_t)(((__int128)row * (row + 1)) / 2);
    ensure_rows(row);
    size_t idx = tri_index((idx_t)row, (idx_t)col);
    return tri[idx];
}
