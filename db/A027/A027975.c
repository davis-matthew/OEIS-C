/*
 * A027975  a(n) is the n-th diagonal sum of left justified array T given by
 *          A027960.
 * https://oeis.org/A027975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + 3 (a(0)=1, a(1)=1, a(2)=4)
 * Range:     u8 n <= 15, u16 n <= 34, u32 n <= 74, u64 n <= 153,
 *            u128 n <= 310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types provided by the build. */
extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    /* Static cache for memoization across calls */
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static size_t cache_size = 0;   /* number of terms currently stored */

    /* Base values */
    const seq_t base[3] = {(seq_t)1, (seq_t)1, (seq_t)4};

    /* Return trivially for the first three indices */
    if (n < 3) {
        return base[(size_t)n];
    }

    /* Ensure the cache can hold index n */
    if (cache_size <= (size_t)n) {
        /* Expand capacity if needed */
        size_t new_cap = (cache_size == 0) ? 3 : cache_size * 2;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out‑of‑memory; for this exercise we simply abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;

        /* Initialize newly created slots if the cache was empty */
        if (cache_size == 0) {
            cache[0] = base[0];
            cache[1] = base[1];
            cache[2] = base[2];
            cache_size = 3;
        }

        /* Compute all missing terms up to n */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = cache[i - 2] + cache[i - 3] + (seq_t)3;
        }
        cache_size = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
