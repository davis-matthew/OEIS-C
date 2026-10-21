/*
 * A063350  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          77 ).
 * https://oeis.org/A063350
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   For n ≥ 14, a(n) = a(n-4) + a(n-6) – a(n-10), with initial
 *            terms: a(2) = -1, a(3) = 179, a(4) = 408, a(5) = 634, a(6) =
 *            860, a(7) = 1086, a(8) = 1312, a(9) = 1538, a(10) = 1764, a(11)
 *            = 1990, a(12) = 2216, a(13) = 2446
 * Range:     i8 n <= 2, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle indices below the offset arbitrarily */
    if (n < 2) return 0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;   /* highest n for which we have a value */
    static size_t capacity = 0;    /* allocated size of cache array */

    /* Initial base values (indices 2..13 inclusive) */
    static const seq_t base_vals[] = {
        -1,    /* a(2) */
         179,  /* a(3) */
         408,  /* a(4) */
         634,  /* a(5) */
         860,  /* a(6) */
        1086,  /* a(7) */
        1312,  /* a(8) */
        1538,  /* a(9) */
        1764,  /* a(10) */
        1990,  /* a(11) */
        2216,  /* a(12) */
        2446   /* a(13) */
    };
    const idx_t base_start = 2;
    const idx_t base_end   = 13;
    const size_t base_size = sizeof(base_vals) / sizeof(base_vals[0]);  /* 12 */

    /* Initialise cache on first call */
    if (cache == NULL) {
        capacity = base_size;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) exit(1);  /* Allocation failed */
        for (size_t i = 0; i < base_size; ++i)
            cache[i] = base_vals[i];
        max_cached = base_end;
    }

    /* If the requested term is already cached, return it */
    if (n <= max_cached) {
        return cache[n - base_start];
    }

    /* Expand the cache as needed */
    if (n - base_start + 1 > capacity) {
        /* Grow capacity to at least the needed size */
        size_t new_capacity = capacity * 2;
        while (new_capacity < n - base_start + 1)
            new_capacity *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (new_cache == NULL) exit(1);  /* Allocation failed */
        cache = new_cache;
        capacity = new_capacity;
    }

    /* Compute missing values sequentially up to n */
    for (idx_t idx = max_cached + 1; idx <= n; ++idx) {
        seq_t val;
        if (idx <= base_end) {                /* Should never happen beyond base */
            val = base_vals[idx - base_start];
        } else {
            /* Recurrence: a(idx-4) + a(idx-6) - a(idx-10) */
            seq_t v1 = cache[(idx - 4) - base_start];
            seq_t v2 = cache[(idx - 6) - base_start];
            seq_t v3 = cache[(idx -10) - base_start];
            val = v1 + v2 - v3;
        }
        cache[idx - base_start] = val;
    }
    max_cached = n;
    return cache[n - base_start];
}
