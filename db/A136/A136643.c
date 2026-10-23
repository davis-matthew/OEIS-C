/*
 * A136643  Triangle of coefficients of characteristic polynomials of
 *          anti-symmetrical tridiagonal matrices: Middle diagonal: a=1; Lower
 *          first subdiagonal: b=2; Upper first subdiagonal: c=-2; Example:
 *          M(3) {{1, -2, 0}, {2, 1, -2}, {0, 2, 1}}.
 * https://oeis.org/A136643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A136643 – Triangle of coefficients of characteristic polynomials
    of anti‑symmetrical tridiagonal matrices (a=1, b=2, c=−2).
    The triangle rows are
        row r = P_{r-1}  if r is odd
        row r = –P_{r-1} if r is even
    where the polynomials P_k satisfy  
        P_0 = 1
        P_1 = x – 1
        P_k = (x – 1) P_{k-1} + 4 P_{k-2}   (k ≥ 2)
    The sequence a(n) is the flattened triangle (row by row, left to right). */

#include <stdlib.h>
#include <string.h>

/* Global cache for the flattened triangle */
static seq_t *cache      = NULL;    /* flattened values 1‑based */
static size_t cache_cap  = 0;       /* allocated capacity */
static size_t cache_len  = 0;       /* number of stored terms */

/* Polynomials P_{k-2} and P_{k-1} used to generate P_k */
static seq_t *poly_prevprev = NULL; /* degree k-2 */
static size_t poly_sz_prevprev = 0;

static seq_t *poly_prev    = NULL; /* degree k-1 */
static size_t poly_sz_prev   = 0;

/* Ensure the cache has at least 'need' elements */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache && cache_len)
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Append 'cnt' terms from 'src' to the cache */
static void append_terms(const seq_t *src, size_t cnt, int sign)
{
    ensure_cache(cache_len + cnt);
    for (size_t i = 0; i < cnt; ++i) {
        seq_t v = src[i];
        if (sign < 0) v = (seq_t)(-v);
        cache[cache_len++] = v;
    }
}

seq_t a(idx_t n)
{
    /* 1‑based indexing – return 0 for n == 0 (offset is 1) */
    if (n == 0) return (seq_t)0;

    /* If we are already computed up to n, just return the value. */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* First call: initialise polynomials and cache */
    if (cache_len == 0) {
        /* P_0 = 1  (degree 0) */
        poly_prev     = (seq_t*)malloc(sizeof(seq_t));
        poly_sz_prev  = 1;
        poly_prev[0]  = (seq_t)1;

        poly_prevprev = NULL;          /* degree -1 (empty) */
        poly_sz_prevprev = 0;

        /* Append first row (row 1) – P_0 with sign +1 (odd row) */
        append_terms(poly_prev, 1, 1);
    }

    /* Generate further rows until we have at least n terms */
    /* current_degree = degree of poly_prev (row index = current_degree + 1) */
    idx_t current_degree = poly_sz_prev - 1; /* since degree = size-1 */

    while (cache_len < (size_t)n) {
        idx_t k = current_degree + 1;   /* next degree to compute */
        size_t sz_new = k + 1;          /* number of coefficients */

        /* Allocate array for P_k */
        seq_t *poly_new = (seq_t*)malloc(sz_new * sizeof(seq_t));

        /* Compute P_k using (x-1)*P_{k-1} + 4*P_{k-2} */
        for (size_t i = 0; i < sz_new; ++i) {
            seq_t term = 0;

            /* (x-1)*P_{k-1} contribution */
            if (i > 0 && i - 1 < poly_sz_prev)
                term += poly_prev[i - 1];        /* x*P_{k-1} */
            if (i < poly_sz_prev)
                term -= poly_prev[i];            /* -P_{k-1} */

            /* 4*P_{k-2} contribution (if exists) */
            if (poly_sz_prevprev > 0 && i < poly_sz_prevprev)
                term += (seq_t)4 * poly_prevprev[i];

            poly_new[i] = term;
        }

        /* Determine the sign for the new row: odd row ⇒ +1, even row ⇒ -1 */
        int sign = ((k + 1) % 2 == 0) ? -1 : 1;

        /* Append the new row to the cache */
        append_terms(poly_new, sz_new, sign);

        /* Prepare for next iteration: shift polynomials */
        free(poly_prevprev);
        poly_prevprev = poly_prev;
        poly_sz_prevprev = poly_sz_prev;

        poly_prev = poly_new;
        poly_sz_prev = sz_new;

        current_degree = k;
    }

    /* cache_len should now be >= n */
    return cache[n - 1];
}
