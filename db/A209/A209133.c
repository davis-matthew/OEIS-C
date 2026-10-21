/*
 * A209133  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209134; see the Formula section.
 * https://oeis.org/A209133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   coefficients of the recursive sequence u(n,x), where u(1,x)=1,
 *            v(1,x)=1, u(n,x)=u(n-1,x)+(x+1)*v(n-1,x),
 *            v(n,x)=2x*u(n-1,x)+2x*v(n-1,x). The flattened sequence
 *            a(1),a(2),… is the coefficients of u(n,x) read row‑by‑row (row r
 *            has r coefficients)
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* @generated 2026-10-21 */

#include <stdlib.h>

/* External integer types supplied by the build. */
extern seq_t a(idx_t n);

/*************************************************************************/
/*  Static cache for incremental computation of the flattened sequence  */
/*************************************************************************/
static idx_t     cache_terms = 0;   /* number of flattened terms computed */
static idx_t     cache_rows  = 0;   /* last row number computed */
static seq_t   *cache_seq   = NULL; /* flattened sequence a(1),a(2),…  */
static size_t   cache_cap   = 0;    /* allocated capacity for cache_seq   */

static seq_t   *u_prev = NULL;      /* coefficients of u(row-1,x) */
static seq_t   *v_prev = NULL;      /* coefficients of v(row-1,x) */
static idx_t     prev_len = 0;      /* length of previous row */

/* Ensure enough capacity for the flattened cache_seq. */
static void ensure_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        cache_seq = (seq_t *)realloc(cache_seq, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Main function called by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */

    /* Compute rows until we have at least 'n' flattened terms. */
    while (cache_terms < n) {
        /* --- compute next row ------------------------------------ */
        idx_t r = cache_rows + 1;               /* new row number          */
        idx_t r_len = r;                        /* length of the new row   */

        /* Allocate arrays for the new row. */
        seq_t *u_curr = (seq_t *)malloc(r_len * sizeof(seq_t));
        seq_t *v_curr = (seq_t *)malloc(r_len * sizeof(seq_t));
        if (!u_curr || !v_curr) {
            /* fatal error: allocation failure */
            exit(1);
        }

        /* First row special case (n=1). */
        if (r == 1) {
            u_curr[0] = (seq_t)1;
            v_curr[0] = (seq_t)1;
        } else {
            /* General recurrence for rows r >= 2. */
            for (idx_t k = 0; k < r_len; ++k) {
                /* v_curr[k] = 2*(u_prev[k-1]+v_prev[k-1]), k>=1 ; 0 if k==0 */
                seq_t v_prev_k_1   = (k > 0) ? v_prev[k-1] : (seq_t)0;
                seq_t u_prev_k_1   = (k > 0) ? u_prev[k-1] : (seq_t)0;
                v_curr[k] = (seq_t)2 * (u_prev_k_1 + v_prev_k_1);

                /* u_curr[k] = u_prev[k] + v_prev[k] + v_prev[k-1] (if k>0) */
                seq_t u_prev_k     = (k < prev_len) ? u_prev[k] : (seq_t)0;
                seq_t v_prev_k     = (k < prev_len) ? v_prev[k] : (seq_t)0;
                seq_t add_v_prev_k_1 = (k > 0) ? v_prev[k-1] : (seq_t)0;
                u_curr[k] = u_prev_k + v_prev_k + add_v_prev_k_1;
            }
        }

        /* --- append new row's u coefficients to the flattened cache --- */
        ensure_cache(cache_terms + r_len);
        for (idx_t k = 0; k < r_len; ++k) {
            cache_seq[cache_terms + k] = u_curr[k];
        }
        cache_terms += r_len;
        cache_rows = r;

        /* Free previous row arrays and set up for next iteration. */
        free(u_prev);
        free(v_prev);
        u_prev = u_curr;
        v_prev = v_curr;
        prev_len = r_len;
    }

    /* Return requested term (1‑based). */
    return cache_seq[n - 1];
}
