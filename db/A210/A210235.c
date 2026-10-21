/*
 * A210235  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210236; see the Formula section.
 * https://oeis.org/A210235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A210235: Triangle of coefficients of polynomials u(n,x). */
/* a(1)=1; subsequent terms are the coefficients of u(n,x) listed
   left‑to‑right, top‑to‑bottom. */

#include <stdlib.h>
#include <stddef.h>

static void grow_cache(size_t needed)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);                /* allocation failed */
        cache = new_arr;
        cache_cap = new_cap;
    }
    /* expose cache and len to the caller */
    /* they are static inside this file and will be accessed in a() */
    /* by a helper wrapper below. */
    /* We'll provide wrapper functions to get/set values. */
    /* Not needed here because `a` will access static variables directly. */
    return;
}

static seq_t *u_prev = NULL, *v_prev = NULL;
static size_t prev_len = 0;          /* length of previous row */
static seq_t *cached_vals = NULL;
static size_t cached_len = 0, cached_cap = 0;

static void ensure_cache(size_t needed)
{
    if (needed > cached_cap) {
        size_t new_cap = cached_cap ? cached_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        seq_t *new_arr = (seq_t *)realloc(cached_vals, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        cached_vals = new_arr;
        cached_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, but safe */

    if (n <= cached_len) return cached_vals[n-1];

    if (cached_len == 0) {
        /* initialize first row */
        prev_len = 1;
        u_prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        v_prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
        u_prev[0] = (seq_t)1;
        v_prev[0] = (seq_t)1;
        ensure_cache(1);
        cached_vals[0] = u_prev[0];
        cached_len = 1;
    }

    for (idx_t cur_row = (idx_t)prev_len + 1; cur_row <= n; ++cur_row) {
        size_t cur_len = (size_t)cur_row;
        seq_t *u_curr = (seq_t *)malloc(cur_len * sizeof(seq_t));
        seq_t *v_curr = (seq_t *)malloc(cur_len * sizeof(seq_t));

        /* compute coefficients for the new row */
        for (size_t j = 0; j < cur_len; ++j) {
            /* u_curr */
            if (j == 0) {
                u_curr[j] = v_prev[0] + (seq_t)1;
            } else if (j < prev_len) {
                u_curr[j] = u_prev[j-1] + v_prev[j];
            } else { /* j == prev_len */
                u_curr[j] = u_prev[j-1];
            }

            /* v_curr */
            if (j == 0) {
                v_curr[j] = u_prev[0] + v_prev[0] + (seq_t)1;
            } else if (j < prev_len) {
                v_curr[j] = u_prev[j-1] + u_prev[j] + v_prev[j-1] + v_prev[j];
            } else { /* j == prev_len */
                v_curr[j] = u_prev[j-1] + v_prev[j-1];
            }
        }

        /* append u_curr coefficients to cache */
        ensure_cache(cached_len + cur_len);
        for (size_t j = 0; j < cur_len; ++j) {
            cached_vals[cached_len++] = u_curr[j];
        }

        /* free previous row arrays and update pointers */
        free(u_prev);
        free(v_prev);
        u_prev = u_curr;
        v_prev = v_curr;
        prev_len = cur_len;
    }

    return cached_vals[n-1];
}
