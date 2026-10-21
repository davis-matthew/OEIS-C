/*
 * A209161  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209160; see the Formula section.
 * https://oeis.org/A209161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in v(r,x) where r and k are
 *            determined by the row‑major indexing of the triangle defined by
 *            u(1,x) = 1, v(1,x) = 1 u(n,x) = u(n-1,x) + (x+1)*v(n-1,x) v(n,x)
 *            = 2x*u(n-1,x) + 2x*v(n-1,x) + 1 The triangle rows are numbered
 *            1,2,3,… and each row r has r coefficients (v(r...
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* Helper: ensure that the cache can hold indices up to 'need' (0‑based size). */
static void _ensure_cache(size_t need, seq_t **cache, size_t *cap)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                     /* allocation failure */
    *cache = tmp;
    *cap   = new_cap;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    /* Static state: cache of coefficients, and the current triangle row. */
    static seq_t *cache   = NULL;   /* 1‑based indexing: cache[i] = a(i) */
    static size_t cache_cap = 0;
    static idx_t  max_row   = 0;     /* highest row that has been computed */
    static seq_t *u_prev = NULL;    /* coefficients of u for the previous row */
    static seq_t *v_prev = NULL;    /* coefficients of v for the previous row */

    if (n == 0) return (seq_t)0;    /* out of range */

    /* Determine which row contains the desired index. */
    idx_t r = (idx_t)((sqrt((long double)8.0L * n + 1.0L) - 1.0L) / 2.0L);
    while (r * (r + 1) / 2 < n) ++r;   /* bump up in case of rounding error */

    /* Initialise the first row if this is the first call. */
    if (max_row == 0) {
        u_prev = (seq_t *)malloc(sizeof(seq_t));
        v_prev = (seq_t *)malloc(sizeof(seq_t));
        u_prev[0] = (seq_t)1;
        v_prev[0] = (seq_t)1;
        _ensure_cache(2, &cache, &cache_cap);   /* need space for index 1 */
        cache[1] = v_prev[0];
        max_row = 1;
    }

    /* Compute rows until we reach the needed one. */
    while (max_row < r) {
        idx_t next = max_row + 1;          /* row to compute */
        /* Allocate new coefficient arrays for the next row. */
        seq_t *new_u = (seq_t *)malloc(sizeof(seq_t) * next);
        seq_t *new_v = (seq_t *)malloc(sizeof(seq_t) * next);

        /* Compute new_u[j] = u_prev[j] + v_prev[j] + v_prev[j‑1] (with bounds). */
        for (idx_t j = 0; j < next; ++j) {
            seq_t a = (j < max_row) ? u_prev[j] : (seq_t)0;
            seq_t b = (j < max_row) ? v_prev[j] : (seq_t)0;
            seq_t c = (j > 0) ? v_prev[j-1] : (seq_t)0;
            new_u[j] = a + b + c;
        }

        /* Compute new_v.  new_v[0] = 1;  new_v[j] = 2 * (u_prev[j‑1] + v_prev[j‑1]) (j≥1). */
        new_v[0] = (seq_t)1;
        for (idx_t j = 1; j < next; ++j) {
            new_v[j] = ((seq_t)2) * (u_prev[j-1] + v_prev[j-1]);
        }

        /* Cache the new row's coefficients into the flat array. */
        idx_t prev_last = max_row * (max_row + 1) / 2;
        idx_t new_last  = next * (next + 1) / 2;
        _ensure_cache((size_t)new_last + 1, &cache, &cache_cap);
        for (idx_t j = 0; j < next; ++j) {
            cache[prev_last + 1 + j] = new_v[j];
        }

        /* Prepare for next iteration: free old arrays and set to new ones. */
        free(u_prev);
        free(v_prev);
        u_prev = new_u;
        v_prev = new_v;
        max_row = next;
    }

    return cache[n];
}
