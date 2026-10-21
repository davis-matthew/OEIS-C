/*
 * A221783  Number of nX4 arrays of occupancy after each element moves to some
 *          horizontal or antidiagonal neighbor, without 2-loops or left
 *          turns.
 * https://oeis.org/A221783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-2) + 5*a(n-3) + a(n-5) * Initial values: a(1)=0,
 *            a(2)=3, a(3)=5, a(4)=16, a(5)=41
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Computes a(n) for the OEIS sequence A221783.
 * Supports any order of calls.  Results are memoised.
 */
seq_t a(idx_t n) {
    /* Handle offset: if called with n = 0, return 0 (undefined in OEIS). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Lazy initialisation and memoisation cache */
    static seq_t *cache = NULL;       /* stores a(1) .. a(cached_max) at indices 0..cached_max-1 */
    static idx_t  cached_max = 0;     /* highest n for which cache contains values */

    /* If cache not yet initialised, create space for first 5 values */
    if (cache == NULL) {
        /* Allocate space for the 5 base terms */
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed – unlikely for small n */

        /* Store the five known initial terms */
        cache[0] = (seq_t)0;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache[2] = (seq_t)5;   /* a(3) */
        cache[3] = (seq_t)16;  /* a(4) */
        cache[4] = (seq_t)41;  /* a(5) */
        cached_max = 5;
    }

    /* Return the cached value if already computed */
    if (n <= cached_max) {
        return cache[n - 1];
    }

    /* Extend the cache to accommodate value up to n */
    idx_t old_max = cached_max;
    idx_t new_max = n;
    seq_t *new_cache = (seq_t *)malloc(new_max * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;  /* allocation failed – unlikely for n up to 10⁴ */

    /* Copy existing values */
    memcpy(new_cache, cache, old_max * sizeof(seq_t));

    /* Compute missing terms using the recurrence
     * a(k) = 5*a(k-2) + 5*a(k-3) + a(k-5)
     *
     * In the cache: new_cache[i-1] = a(i)
     * So for i >= old_max+1 >= 6:
     *   a(i-2) -> new_cache[i-3]
     *   a(i-3) -> new_cache[i-4]
     *   a(i-5) -> new_cache[i-6]
     */
    for (idx_t i = old_max + 1; i <= new_max; ++i) {
        seq_t val;
        if (i <= 5) {
            /* Handle small indices that might be computed when the cache
             * is extended beyond 5 for the first time (unlikely but safe). */
            static const seq_t init_vals[5] = {0, 3, 5, 16, 41};
            val = init_vals[i - 1];
        } else {
            val = (seq_t)5 * new_cache[i - 3]            /* a(i-2) */
                + (seq_t)5 * new_cache[i - 4]            /* a(i-3) */
                + new_cache[i - 6];                     /* a(i-5) */
        }
        new_cache[i - 1] = val;
    }

    /* Replace old cache with the new, larger one */
    free(cache);
    cache = new_cache;
    cached_max = new_max;

    return cache[n - 1];
}
