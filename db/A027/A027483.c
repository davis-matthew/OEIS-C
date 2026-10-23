/*
 * A027483  Second subdiagonal of triangle A027478, constructed from the
 *          Stirling numbers of the first kind.
 * https://oeis.org/A027483
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equation: a(n) = A027478(n, n + 2)
   Sequence: Second subdiagonal of triangle A027478 (Stirling numbers of the
   first kind).  Recurrence (n > 11):
        a(n) =  9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4)
               +126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 3)                /* the offset is 3; below that is undefined   */
        return (seq_t)0;

    /* Static cache for memoisation (indices 0 … max_computed) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* array length */
    static idx_t max_computed = 0;   /* largest n for which cache[n] is valid */
    static bool init_done = false;   /* have we initialised a(3)…a(11)? */

    /* Ensure the cache array is large enough for the requested n */
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        cache_size = new_size;
    }

    /* Initialise the storages for a(3) .. a(11) once */
    if (!init_done) {
        /* If the current array is too small, enlarge it */
        if (cache_size < 12) {
            size_t new_size = 12;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (tmp) cache = tmp;
            cache_size = new_size;
        }

        static const seq_t init[9] = {
            176, 2951, 22535, 112435, 426475,
            1333906, 3614226, 8762370, 19439970
        };
        for (idx_t i = 3; i <= 11; ++i)
            cache[i] = init[i - 3];

        /* initialise the unused leading positions to zero for safety */
        for (idx_t i = 0; i < 3; ++i)
            cache[i] = 0;

        max_computed = 11;
        init_done = true;
    }

    if (n <= max_computed)   /* already computed value */
        return cache[n];

    /* Compute further values using the 9‑term linear recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t pos = cache[i - 1] * (seq_t)9
                  + cache[i - 3] * (seq_t)84
                  + cache[i - 5] * (seq_t)126
                  + cache[i - 7] * (seq_t)36
                  + cache[i - 9] * (seq_t)1;

        seq_t neg = cache[i - 2] * (seq_t)36
                  + cache[i - 4] * (seq_t)126
                  + cache[i - 6] * (seq_t)84
                  + cache[i - 8] * (seq_t)9;

        cache[i] = pos - neg;
    }
    max_computed = n;

    return cache[n];
}
