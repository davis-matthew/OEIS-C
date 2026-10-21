/*
 * A210191  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210192; see the Formula section.
 * https://oeis.org/A210191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of u(n,x) from A210192.
   a(n) is the flattened list of the constant, x, x², … coefficients of
   each u(n,x) in row‑major order.  The polynomials are defined by
   u(1,x) = 1, v(1,x) = 1
   u(n,x) = u(n-1,x) + v(n-1,x) + 1
   v(n,x) = 2x*u(n-1,x) + x*v(n-1,x) + 1   (n ≥ 2)
   This function builds the triangle on demand, caches the results,
   and returns the n‑th coefficient (1‑based).  All arithmetic is
   performed in seq_t; standard reallocation casts are used for C++.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached values of the sequence */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* State of the previous row: U and V polynomial coefficients */
static seq_t *prev_u      = NULL;
static seq_t *prev_v      = NULL;
static size_t prev_u_len  = 0;
static size_t prev_v_len  = 0;
static idx_t  rows_cached = 0;

/* Grow the cache and store a new value */
static void cache_append(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the nth term of A210191 (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undefined for 0 */

    while (cache_len < (size_t)n) {
        /* Compute next row k = rows_cached + 1 */
        ++rows_cached;
        idx_t k = rows_cached;

        size_t u_len = (k == 1 ? 1 : (size_t)k - 1);   /* coeffs of U(k,x) */
        size_t v_len = (size_t)k;                     /* coeffs of V(k,x) */

        seq_t *u_new = (seq_t *)malloc(sizeof(seq_t) * u_len);
        seq_t *v_new = (seq_t *)malloc(sizeof(seq_t) * v_len);
        if (!u_new || !v_new) exit(EXIT_FAILURE);

        if (k == 1) {                      /* base row */
            u_new[0] = (seq_t)1;
            v_new[0] = (seq_t)1;
        } else {
            /* Coefficients of U(k,x) */
            for (size_t d = 0; d < u_len; ++d) {
                seq_t pu = (d < prev_u_len ? prev_u[d] : (seq_t)0);
                seq_t pv = (d < prev_v_len ? prev_v[d] : (seq_t)0);
                if (d == 0)
                    u_new[d] = pu + pv + (seq_t)1;
                else
                    u_new[d] = pu + pv;
            }

            /* Coefficients of V(k,x) */
            v_new[0] = (seq_t)1;
            for (size_t d = 1; d < v_len; ++d) {
                seq_t pu = (d-1 < prev_u_len ? prev_u[d-1] : (seq_t)0);
                seq_t pv = (d-1 < prev_v_len ? prev_v[d-1] : (seq_t)0);
                v_new[d] = ((seq_t)2 * pu) + pv;
            }
        }

        /* Store U(k,x) coefficients into the cache */
        for (size_t d = 0; d < u_len; ++d) {
            cache_append(u_new[d]);
        }

        /* Prepare for next iteration */
        if (prev_u) free(prev_u);
        if (prev_v) free(prev_v);
        prev_u = u_new;
        prev_v = v_new;
        prev_u_len = u_len;
        prev_v_len = v_len;
    }

    return cache[(size_t)n - 1];
}
