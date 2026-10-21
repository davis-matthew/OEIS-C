/*
 * A209137  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209138; see the Formula section.
 * https://oeis.org/A209137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of polynomials u(n,x) defined by
 *            u(1,x)=1, v(1,x)=1 and u(n,x)=u(n‑1,x)+(x+1)v(n‑1,x),
 *            v(n,x)=(x+1)u(n‑1,x)+xv(n‑1,x). a(n) is the n‑th coefficient
 *            read row by row (rows start at 1)
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache of already computed terms of the sequence */
static seq_t *cache = NULL;          /* flat array of coefficients */
static idx_t cache_len = 0;          /* number of terms stored in cache */

/* data for the last two computed rows of the triangle */
static seq_t *row_u = NULL;          /* coefficients of u(n,x)   */
static seq_t *row_v = NULL;          /* coefficients of v(n,x)   */
static idx_t row_len = 0;            /* length of the current row (n) */
static idx_t rows_done = 0;          /* number of rows fully processed */

/* helper: extend the triangle up to row 'target' and fill cache */
static void extend_up_to(idx_t target)
{
    if (target <= rows_done)
        return;

    /* initialise first row explicitly */
    if (rows_done == 0) {
        /* row 1: u=[1], v=[1] */
        row_len = 1;
        row_u = (seq_t *)malloc(sizeof(seq_t));
        row_v = (seq_t *)malloc(sizeof(seq_t));
        row_u[0] = (seq_t)1;
        row_v[0] = (seq_t)1;
        /* cache the coefficient of row 1 */
        cache = (seq_t *)realloc(cache, sizeof(seq_t));
        cache[0] = row_u[0];
        cache_len = 1;
        rows_done = 1;
    }

    /* iteratively generate subsequent rows */
    for (idx_t n = rows_done + 1; n <= target; ++n) {
        idx_t new_len = n;                      /* length of new row */
        seq_t *new_u = (seq_t *)calloc(new_len, sizeof(seq_t));
        seq_t *new_v = (seq_t *)calloc(new_len, sizeof(seq_t));

        /* compute coefficients for new_u and new_v */
        for (idx_t k = 0; k < new_len; ++k) {
            /* u(n) = u(n‑1) + (x+1)v(n‑1) */
            seq_t term_u = 0;
            if (k < row_len) term_u += row_u[k];
            if (k < row_len) term_u += row_v[k];           /* x+1 * v -> x*v part */
            if (k > 0)     term_u += row_v[k - 1];          /* +v part */
            new_u[k] = term_u;

            /* v(n) = (x+1)u(n‑1) + x v(n‑1) */
            seq_t term_v = 0;
            if (k < row_len) term_v += row_u[k];           /* +v part will fit below */
            if (k > 0)       term_v += row_u[k - 1];        /* x+1 * u -> 1* part */
            if (k > 0)       term_v += row_v[k - 1];        /* x * v part */
            new_v[k] = term_v;
        }

        /* append new row's coefficients to cache */
        cache = (seq_t *)realloc(cache, (cache_len + new_len) * sizeof(seq_t));
        for (idx_t k = 0; k < new_len; ++k)
            cache[cache_len + k] = new_u[k];
        cache_len += new_len;

        /* free old row arrays and move new ones in place */
        free(row_u);
        free(row_v);
        row_u = new_u;
        row_v = new_v;
        row_len = new_len;
        rows_done = n;
    }
}

/* public function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety guard, not used by OEIS offset */
    /* ensure cache contains at least n terms */
    extend_up_to((idx_t)((-1 + (long long) (long double) 8 * n) /
                       (long double) 2 + 2));  /* worst‑case estimate of row */
    /* fallback: extend until cache_len >= n */
    while (cache_len < n)
        extend_up_to(rows_done + 1);

    return cache[n - 1];
}
