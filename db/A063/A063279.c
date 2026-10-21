/*
 * A063279  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          6 ).
 * https://oeis.org/A063279
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = dimension of the space of weight n cuspidal newforms for
 *            Γ₁(6). The sequence satisfies the following linear recurrence
 *            (obtained from the provided generating function): a(n) = a(n-1)
 *            - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8) for n > 11 with
 *            initial values: a(2) = -1, a(3) = 0, a(4) =...
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values.  cache[i] holds a(i) for i < cache_cap.  */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Initial terms for indices 0 .. 11 (a(0) and a(1) are set to 0).  */
static const seq_t init_vals[12] = {
    0,    /* a(0) - unused but set to 0 */
    0,    /* a(1) - unused but set to 0 */
   -1,    /* a(2) */
    0,    /* a(3) */
    0,    /* a(4) */
    1,    /* a(5) */
    2,    /* a(6) */
    1,    /* a(7) */
    2,    /* a(8) */
    1,    /* a(9) */
    2,    /* a(10) */
    1     /* a(11) */
};

seq_t a(idx_t n) {
    if (n < 2) {
        /* The sequence is defined only for n ≥ 2.  Return 0 for smaller n. */
        return (seq_t)0;
    }

    /* Expand the cache if necessary.  */
    if (n + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        cache_cap = (size_t)n + 1;

        /* Reallocate the array to the new size.  */
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a production build you might want to handle out-of‑memory
               more gracefully; for this exercise we simply abort. */
            abort();
        }
        cache = tmp;

        /* Initialise the new entries.  */
        for (size_t i = old_cap; i < cache_cap; ++i) {
            if (i < 12) {
                cache[i] = init_vals[i];
            } else {
                /* For i ≥ 12 the recurrence has no RHS term. */
                cache[i] = cache[i-1]
                           - cache[i-3]
                           + 2 * cache[i-4]
                           - cache[i-5]
                           + cache[i-7]
                           - cache[i-8];
            }
        }
    }

    return cache[(size_t)n];
}
