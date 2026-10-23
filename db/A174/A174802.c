/*
 * A174802  Triangular sequence from antidiagonal expansion of: p(x,m) = x*(x
 *          + 1)^(m - 1)/(1 - Sum[x^i, {i, 1, m}]).
 * https://oeis.org/A174802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) is the (n, a(n)) value of triangle T(r, m) = coefficient of
 *            x^{r-m+1} in p(x, m) = x*(x+1)^{m-1} / (1 - (x + x^2 + ... +
 *            x^m)) The sequence is read row by row, where row r has r entries
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"

/* global storage for the precomputed triangle */
static seq_t **triangle = NULL;            /* triangle[r][m] */
static idx_t  triangle_rows = 0;           /* number of rows already computed */

/* compute the triangle up to at least 'rows_needed' rows */
static void ensure_rows(idx_t rows_needed)
{
    if (triangle_rows >= rows_needed)
        return;

    /* grow the top-level array */
    if (!triangle)
        triangle = (seq_t**)malloc((rows_needed + 1) * sizeof(seq_t*));
    else
        triangle = (seq_t**)realloc(triangle, (rows_needed + 1) * sizeof(seq_t*));

    /* allocate the new rows */
    for (idx_t r = triangle_rows + 1; r <= rows_needed; ++r)
        triangle[r] = (seq_t*)malloc((r + 1) * sizeof(seq_t)); /* index 0 unused */

    /* compute all rows up to rows_needed */
    for (idx_t m = 1; m <= rows_needed; ++m) {
        /* temporary arrays for the series a[n] and a prefix sum */
        seq_t *a   = (seq_t*)malloc((rows_needed + 1) * sizeof(seq_t));
        seq_t *pref = (seq_t*)malloc((rows_needed + 1) * sizeof(seq_t));

        a[0] = 0;
        pref[0] = 0;

        for (idx_t n = 1; n <= rows_needed; ++n) {
            seq_t N = 0;
            if (n <= m)
                N = binom(m - 1, n - 1);          /* coefficient of x^n in numerator */

            seq_t sumPrev = pref[n - 1];
            if (n > m)
                sumPrev -= pref[n - m - 1];       /* sum of the last m terms */

            a[n] = N + sumPrev;
            pref[n] = pref[n - 1] + a[n];
        }

        /* fill triangle[r][m] = coefficient of x^{r-m+1} = a[exp] */
        for (idx_t r = m; r <= rows_needed; ++r) {
            idx_t exp = r - m + 1;
            triangle[r][m] = a[exp];
        }

        free(a);
        free(pref);
    }

    triangle_rows = rows_needed;
}

/* compute the n-th term of A174802 */
seq_t a(idx_t n)
{
    /* find the row r that contains position n */
    idx_t r = 1;
    while (((__uint128_t)r * (r + 1)) / 2 < n)
        ++r;

    /* column m within that row */
    idx_t m = n - ((idx_t)((__uint128_t)r * (r - 1) / 2));

    /* make sure the triangle has been computed */
    ensure_rows(r);

    return triangle[r][m];
}
