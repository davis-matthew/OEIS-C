/*
 * A209757  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A013609; see the Formula section.
 * https://oeis.org/A209757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^k in v(n,x) where v(n,x) is defined by
 *            u(1,x)=1, v(1,x)=1 u(n,x)=x*u(n-1,x)+x*v(n-1,x)+1
 *            v(n,x)=(x+1)*u(n-1,x)+(x+1)*v(n-1,x)+1 The sequence lists the
 *            non‑zero coefficients of v(n,x) row‑by‑row
 * Range:     u8 n <= 24, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Cached data ------------------------------------------------------------- */
static seq_t *v_cache = NULL;      /* sequence values a(1), a(2), …          */
static size_t cache_size = 0;      /* number of cached terms               */
static size_t cache_cap  = 0;      /* allocated capacity for cache          */

static seq_t *u_cur = NULL;       /* coefficients of u for current row    */
static seq_t *v_cur = NULL;       /* coefficients of v for current row    */
static size_t cur_len = 0;        /* length of current row                */

/* Helper to ensure cache has space for `need` terms. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need)
    {
        size_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap < need) new_cap <<= 1;
        v_cache = (seq_t *)realloc(v_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Heap allocation for polynomial arrays of given length. */
static void init_polynomial_arrays(size_t len)
{
    u_cur = (seq_t *)malloc(len * sizeof(seq_t));
    v_cur = (seq_t *)malloc(len * sizeof(seq_t));
}

/* Main function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* n < 1 is outside the OEIS offset */

    /* Lazy initialization of the first row (n = 1). */
    if (cur_len == 0)
    {
        cur_len = 1;
        init_polynomial_arrays(cur_len);
        u_cur[0] = 1;
        v_cur[0] = 1;

        ensure_cache_capacity(1);
        v_cache[0] = 1;
        cache_size = 1;
    }

    /* Extend the triangle until we have at least n terms. */
    while (cache_size < (size_t)n)
    {
        size_t next_len = cur_len + 1;  /* new row length */

        /* Allocate arrays for the next row. */
        seq_t *u_next = (seq_t *)malloc(next_len * sizeof(seq_t));
        seq_t *v_next = (seq_t *)malloc(next_len * sizeof(seq_t));

        /* Compute first coefficient of the new row. */
        u_next[0] = 1;
        v_next[0] = u_cur[0] + v_cur[0] + 1;

        /* Compute the remaining coefficients. */
        for (size_t k = 1; k < next_len; ++k)
        {
            seq_t up_k_1 = u_cur[k-1];
            seq_t vp_k_1 = v_cur[k-1];
            seq_t up_k   = (k < cur_len) ? u_cur[k] : 0;
            seq_t vp_k   = (k < cur_len) ? v_cur[k] : 0;

            u_next[k] = up_k_1 + vp_k_1;
            v_next[k] = up_k_1 + vp_k_1 + up_k + vp_k;
        }

        /* Store the new row's coefficients into the cache. */
        ensure_cache_capacity(cache_size + next_len);
        for (size_t k = 0; k < next_len; ++k)
        {
            v_cache[cache_size++] = v_next[k];
        }

        /* Free previous row arrays and move to the new row. */
        free(u_cur);
        free(v_cur);
        u_cur = u_next;
        v_cur = v_next;
        cur_len = next_len;
    }

    /* Return the requested term (1‑based indexing). */
    return v_cache[n-1];
}
