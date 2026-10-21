/*
 * A063343  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          70 ).
 * https://oeis.org/A063343
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A063343: dimension of the space of weight n cuspidal newforms for Gamma_1(70)
   Recurrence: a(n) = a(n-4) + a(n-6) - a(n-10)  for n > 13
   Base values (offset 2): a(2) = -1, a(3) = 45 ... a(13) = 462
   The function caches computed terms for efficiency and works for any n ≥ 2. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle indices below the offset */
    if (n < 2) return 0;

    /* Base values up to n = 13 (index 13 inclusive) */
    static const seq_t base_vals[] = {
        0,          /* a(0) unused */
        0,          /* a(1) unused */
       -1,          /* a(2) */
        45,         /* a(3) */
        84,         /* a(4) */
        126,        /* a(5) */
        168,        /* a(6) */
        210,        /* a(7) */
        252,        /* a(8) */
        298,        /* a(9) */
        336,        /* a(10) */
        382,        /* a(11) */
        420,        /* a(12) */
        462          /* a(13) */
    };

    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_max = 0;   /* maximum index computed so far */

    /* Ensure the cache is initialized to hold the base values */
    if (cache == NULL) {
        cached_max = sizeof(base_vals) / sizeof(base_vals[0]) - 1; /* 13 */
        cache = (seq_t *)malloc((cached_max + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= cached_max; ++i)
            cache[i] = base_vals[i];
    }

    /* Grow cache if necessary */
    if (n > cached_max) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) /* allocation failure */
            return 0; /* fallback; normally should handle error */
        cache = new_cache;

        /* Compute new terms using recurrence */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            /* Recurrence holds for i > 13 ; for i <= 13 we already have base values */
            if (i > 13)
                cache[i] = cache[i-4] + cache[i-6] - cache[i-10];
            else
                cache[i] = base_vals[i];   /* redundant but harmless */
        }
        cached_max = n;
    }

    return cache[n];
}
