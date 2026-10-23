/*
 * A136644  Triangle of coefficients of characteristic polynomials of
 *          asymmetrical tridiagonal matrices: Middle diagonal: a=1; Lower
 *          first subdiagonal: b=2; Upper first subdiagonal: c=1; Example:
 *          M(3) {{1, 1, 0}, {2, 1, 1}, {0, 2, 1}}.
 * https://oeis.org/A136644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   a(n) is the flattened triangle of coefficients of the *
 *            characteristic polynomials of the asymmetrical tridiagonal
 *            matrices * with middle diagonal a=1, lower first subdiagonal
 *            b=2, upper first * subdiagonal c=1. Row n uses the polynomial
 *            p_{n‑1} of degree n‑1, * written with ascending coefficient...
 * Range:     i8 n <= 48, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* external types supplied by the build environment */
 /* seq_t : signed integer or arbitrary‑precision class */
 /* idx_t : unsigned 64‑bit */

/* Cached flattened sequence */
static seq_t *cached_seq   = NULL;   /* array of flattened terms          */
static size_t  cached_sz   = 0;      /* number of stored terms            */
static size_t  cached_cap  = 0;      /* capacity of cached_seq            */

/* Stored ascending polynomials p_k (0‑based).  polys[0] stores p_0,
 * polys[1] stores p_1, etc.  polys_cap is the allocated size. */
static seq_t **polys       = NULL;   /* array of pointers to polynomials  */
static size_t  polys_cap   = 0;      /* capacity of polys array           */
static size_t  polys_cnt   = 0;      /* number of stored polynomials      */

/* Ensure cached_seq has capacity at least needed */
static void ensure_cache_capacity(size_t needed)
{
    if (cached_cap < needed) {
        size_t newcap = cached_cap ? cached_cap * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cached_seq, newcap * sizeof(seq_t));
        cached_seq = tmp;
        cached_cap = newcap;
    }
}

/* Ensure that polys contains the polynomials p_0 … p_k */
static void ensure_polynomial(size_t k)
{
    if (polys_cnt > k) return;   /* already computed */

    /* Allocate polys array if first time */
    if (polys_cap == 0) {
        size_t initcap = 4;
        polys = (seq_t **)malloc(initcap * sizeof(seq_t *));
        polys_cap = initcap;
        polys_cnt = 0;
    }

    /* Resize polys array if necessary */
    if (polys_cnt + 1 > polys_cap) {
        size_t newcap = polys_cap << 1;
        seq_t **tmp = (seq_t **)realloc(polys, newcap * sizeof(seq_t *));
        polys = tmp;
        polys_cap = newcap;
    }

    /* Initial polynomials */
    if (polys_cnt == 0) {          /* p_0 = [1] */
        polys[0] = (seq_t *)malloc(sizeof(seq_t));
        polys[0][0] = (seq_t)1;
        polys_cnt = 1;
    }
    if (polys_cnt == 1) {          /* p_1 = [-1, 1] */
        polys[1] = (seq_t *)malloc(2 * sizeof(seq_t));
        polys[1][0] = (seq_t)-1;
        polys[1][1] = (seq_t)1;
        polys_cnt = 2;
    }

    /* Compute further polynomials up to k */
    while (polys_cnt <= k) {
        size_t idx = polys_cnt;           /* compute p_idx  (idx = k) */
        size_t len_prev1 = idx;           /* length of p_{idx-1}    */
        size_t len_prev2 = idx - 1;       /* length of p_{idx-2}    */

        seq_t *prev1 = polys[idx - 1];
        seq_t *prev2 = polys[idx - 2];

        /* Allocate new polynomial (length idx+1) */
        seq_t *next = (seq_t *)malloc((idx + 1) * sizeof(seq_t));

        /* Leading term of (λ-1)p_{idx-1} part */
        next[0] = (seq_t)-prev1[0];          /* -c[0]  */

        /* Interior terms: c[i-1] - c[i] for i=1..idx-1 */
        for (size_t i = 1; i < idx; ++i) {
            next[i] = prev1[i - 1] - prev1[i];
        }

        /* Constant term of (λ-1)p_{idx-1} */
        next[idx] = prev1[idx - 1];

        /* Subtract 2*p_{idx-2} (shifted by 2) */
        for (size_t j = 0; j < len_prev2; ++j) {
            next[j] -= (seq_t)2 * prev2[j];
        }

        polys[polys_cnt] = next;
        ++polys_cnt;
    }
}

/* Append the polynomial p_{row‑1} to the cache, applying the sign rule. */
static void extend_cache_to_row(size_t row, size_t *rows_done)
{
    ensure_polynomial(row - 1);
    seq_t *poly = polys[row - 1];
    size_t len = row;                                   /* coefficients count */
    ensure_cache_capacity(cached_sz + len);

    if (row & 1) {   /* odd row – keep signs as is */
        for (size_t i = 0; i < len; ++i) {
            cached_seq[cached_sz++] = poly[i];
        }
    } else {         /* even row – multiply by –1 */
        for (size_t i = 0; i < len; ++i) {
            cached_seq[cached_sz++] = -poly[i];
        }
    }

    *rows_done = row;
}

/* Ensure cache contains at least n terms; n>=1 */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)1) return;

    static idx_t rows_done = 0;          /* last finished row */

    while (cached_sz < (size_t)n) {
        size_t next_row = (size_t)(rows_done + 1);
        extend_cache_to_row(next_row, &rows_done);
    }
}

/* Public function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;          /* undefined for n<1 */
    ensure_cache(n);
    return cached_seq[(size_t)(n - 1)];
}
