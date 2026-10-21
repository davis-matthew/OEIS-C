/*
 * A208922  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208921; see the Formula section.
 * https://oeis.org/A208922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of v(n,x) generated from u(1,x)=1,
 *            v(1,x)=1 u(n,x) = u(n-1,x) + 2x*v(n-1,x) v(n,x) = (x+1)u(n-1,x)
 *            + v(n-1,x) + 1 a(n) is the coefficients of v(n,x) listed row by
 *            row
 * Range:     u8 n <= 23, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for memoization across calls */
static seq_t *cache = NULL;          /* flat sequence of a(n) */
static size_t cache_len = 0;         /* number of terms currently stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */

static seq_t *u_prev = NULL;         /* coefficient array for u of previous row */
static seq_t *v_prev = NULL;         /* coefficient array for v of previous row */
static idx_t prev_len = 0;           /* length of previous row (== number of coeffs) */

/* Ensure cache has at least 'need' capacity */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failure - abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure previous row buffers are initialized */
static void init_first_row(void)
{
    if (cache_len > 0) return; /* already initialized */
    /* Row 1: v(1,x) = 1 */
    ensure_cache_cap(1);
    cache[0] = (seq_t)1;
    cache_len = 1;

    /* Allocate row 1 arrays for u and v */
    u_prev = (seq_t *)calloc(1, sizeof(seq_t));
    v_prev = (seq_t *)calloc(1, sizeof(seq_t));
    if (!u_prev || !v_prev) abort();
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
    prev_len = 1;
}

/* Main function required by the specifications */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, return 0 */

    init_first_row();              /* ensure row 1 is present */

    /* Grow the cache until it contains term n */
    while (cache_len < (size_t)n) {
        /* Generate next row: length = prev_len + 1 */
        idx_t cur_len = prev_len + 1;

        /* Allocate arrays for current row */
        seq_t *u_cur = (seq_t *)calloc(cur_len, sizeof(seq_t));
        seq_t *v_cur = (seq_t *)calloc(cur_len, sizeof(seq_t));
        if (!u_cur || !v_cur) abort();

        /* Compute u(cur) coefficients */
        for (idx_t j = 0; j < cur_len; ++j) {
            seq_t t1 = (j < prev_len) ? u_prev[j] : (seq_t)0;
            seq_t t2 = (j > 0) ? (v_prev[j - 1] + v_prev[j - 1]) : (seq_t)0;
            u_cur[j] = t1 + t2;
        }

        /* Compute v(cur) coefficients */
        for (idx_t j = 0; j < cur_len; ++j) {
            seq_t t1 = (j > 0) ? u_prev[j - 1] : (seq_t)0;
            seq_t t2 = (j < prev_len) ? u_prev[j] : (seq_t)0;
            seq_t t3 = (j < prev_len) ? v_prev[j] : (seq_t)0;
            seq_t t4 = (j == 0) ? (seq_t)1 : (seq_t)0;
            v_cur[j] = t1 + t2 + t3 + t4;
        }

        /* Append new coefficients to the linear cache */
        ensure_cache_cap(cache_len + (size_t)cur_len);
        for (idx_t j = 0; j < cur_len; ++j) {
            cache[cache_len + j] = v_cur[j];
        }
        cache_len += (size_t)cur_len;

        /* Prepare for next iteration */
        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
        prev_len = cur_len;
    }

    return cache[(size_t)(n - 1)];
}
