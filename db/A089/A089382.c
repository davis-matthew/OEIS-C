/*
 * A089382  Total number of triangles in all the dissections of a convex
 *          (n+3)-gon by nonintersecting diagonals.
 * https://oeis.org/A089382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   compute_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A089382 – Total number of triangles in all dissections of a convex (n+3)-gon
 *  Recurrence (valid for n ≥ 3):
 *        n*(n+2)*a(n) = (8*n^2 + 7*n - 3)*a(n-1)
 *                      - (13*n^2 - 17*n - 6)*a(n-2)
 *                      + 2*(n-3)*(n+1)*a(n-3)
 *  Initial values:
 *        a(0) = 1, a(1) = 4, a(2) = 20
 *
 *  The function below memoises the computed values and uses the recurrence
 *  to fill missing entries in O(n).  It works for any `seq_t` provided by
 *  the build environment, including arbitrary‑precision types.
 */

#include <stdlib.h>
#include "../utils/compute_term.h"

static seq_t *cache = NULL;          /* pointer to memoised terms           */
static size_t cache_capacity = 0;    /* current allocated size of cache     */
static size_t computed_terms = 0;    /* number of terms actually stored      */

/* ----------------------------------------------------------- */
/* Compute a(k) for k ≥ 3 using the recurrence.                */

/* ----------------------------------------------------------- */
/* Public entry‑point:   seq_t a(idx_t n)                      */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices 0 … n (inclusive) */
    if (n + 1 > cache_capacity) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)          /* allocation failure – abort */
            abort();
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    if (computed_terms <= n) {
        for (idx_t k = computed_terms; k <= n; ++k) {
            seq_t term;
            if (k == 0)      term = (seq_t)1;
            else if (k == 1) term = (seq_t)4;
            else if (k == 2) term = (seq_t)20;
            else             term = compute_term(k, cache);
            cache[k] = term;
        }
        computed_terms = n + 1;
    }

    return cache[n];
}
