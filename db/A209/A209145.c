/*
 * A209145  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A122075; see the Formula section.
 * https://oeis.org/A209145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads off the rows of the triangle of coefficients of the
 *            polynomials u(n,x) defined by u(1,x) = 1 v(1,x) = 1 u(n,x) =
 *            u(n-1,x) + (x+1) * v(n-1,x) v(n,x) = u(n-1,x) + x * v(n-1,x) + 1
 *            The sequence lists the coefficients of u(n,x) row by row,
 *            starting with a(1)=1
 * Range:     u8 n <= 37, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for all computed coefficients */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* static storage for the most recent row of u and v */
    static seq_t *u = NULL;
    static seq_t *v = NULL;
    static idx_t current_row = 0;   /* number of rows already generated */

    /* initialise on first call */
    if (cache_len == 0) {
        cache_cap = 128;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));

        current_row = 1;
        /* row 1: u(1,x)=1, v(1,x)=1 */
        u          = (seq_t *)malloc(sizeof(seq_t));
        v          = (seq_t *)malloc(sizeof(seq_t));
        u[0] = 1;
        v[0] = 1;

        /* cache the only coefficient of the first row */
        cache[0] = u[0];
        cache_len = 1;
    }

    /* generate rows until we have at least n coefficients cached */
    while (cache_len < n) {
        /* length of the new row to produce */
        idx_t new_len = current_row + 1;

        seq_t *next_u = (seq_t *)malloc(new_len * sizeof(seq_t));
        seq_t *next_v = (seq_t *)malloc(new_len * sizeof(seq_t));

        for (idx_t j = 0; j < new_len; ++j) {
            /* previous row has length new_len-1 */
            seq_t u_prev = (j < new_len - 1) ? u[j] : 0;
            seq_t v_prev = (j < new_len - 1) ? v[j] : 0;
            seq_t v_prev_shift = (j > 0) ? v[j - 1] : 0;

            /* u(n,x) = u_prev + v_prev + x*v_prev */
            next_u[j] = u_prev + v_prev + v_prev_shift;

            /* v(n,x) = u_prev + x*v_prev + 1 (only at constant term) */
            next_v[j] = u_prev + v_prev_shift + ((j == 0) ? 1 : 0);
        }

        /* free old arrays and replace them with the new ones */
        free(u);
        free(v);
        u = next_u;
        v = next_v;

        /* ensure cache has enough space for the new coefficients */
        if (cache_len + new_len > cache_cap) {
            while (cache_len + new_len > cache_cap)
                cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* append the new row's coefficients to the cache */
        for (idx_t j = 0; j < new_len; ++j)
            cache[cache_len++] = u[j];

        current_row = new_len;
    }

    return cache[n - 1];
}
