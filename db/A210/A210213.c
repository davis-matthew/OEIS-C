/*
 * A210213  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210214; see the Formula section.
 * https://oeis.org/A210213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   triangle of coefficients of polynomials u(n,x), defined by
 *            u(n,x) = x*u(n-1,x)+v(n-1,x)+1, v(n,x) =
 *            u(n-1,x)+(x+1)*v(n-1,x)+1, with u(1,x)=1, v(1,x)=1. The sequence
 *            a(n) is the flattened triangle, row by row, starting with the
 *            constant term of each polynomial
 * Range:     u8 n <= 30, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* flattened triangle values */
static size_t  cache_len = 0;        /* number of values cached          */
static size_t  cache_cap = 0;        /* allocated capacity of cache       */

static seq_t *u_prev = NULL;         /* coefficients of u for previous row */
static seq_t *v_prev = NULL;         /* coefficients of v for previous row */
static size_t  prev_len = 0;         /* length of previous row           */

/* Append a value to the cache, growing storage if needed */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Generate the next row of u and v polynomials and append u's coefficients
   to the cache.  After execution, u_prev/v_prev point to the new row. */
static void generate_next_row(void)
{
    /* new row length = prev_len + 1 */
    size_t new_len = prev_len + 1;
    seq_t *u_new = (seq_t *)malloc(new_len * sizeof(seq_t));
    seq_t *v_new = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!u_new || !v_new) abort();  /* allocation failure */

    /* k = 0 case */
    u_new[0] = v_prev[0] + (seq_t)1;
    v_new[0] = u_prev[0] + v_prev[0] + (seq_t)1;

    /* 1 <= k <= new_len-2 */
    if (new_len >= 3) {
        for (size_t k = 1; k + 1 < new_len; ++k) {
            u_new[k] = u_prev[k - 1] + v_prev[k];
            v_new[k] = u_prev[k] + v_prev[k] + v_prev[k - 1];
        }
    }

    /* last index k = new_len-1 */
    u_new[new_len - 1] = u_prev[prev_len - 1];
    v_new[new_len - 1] = v_prev[prev_len - 1];

    /* append new row's u coefficients to cache */
    for (size_t k = 0; k < new_len; ++k) {
        append_cache(u_new[k]);
    }

    /* free previous arrays and update pointers/length */
    free(u_prev);
    free(v_prev);
    u_prev = u_new;
    v_prev = v_new;
    prev_len = new_len;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;   /* already enough */

    /* initialize first row if needed */
    if (cache_len == 0) {
        /* Row 1: u = 1, v = 1 */
        prev_len = 1;
        u_prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        v_prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        if (!u_prev || !v_prev) abort();
        u_prev[0] = (seq_t)1;
        v_prev[0] = (seq_t)1;
        append_cache(u_prev[0]);          /* a(1) = 1 */
    }

    /* generate rows until we have enough cached values */
    while (cache_len < (size_t)n) {
        generate_next_row();
    }
}

/* Public function requested by OEIS */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n < 1, return 0 */
    ensure_cache(n);
    return cache[n - 1];
}
