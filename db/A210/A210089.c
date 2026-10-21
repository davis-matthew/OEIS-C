/*
 * A210089  Number of (n+1) X 3 0..2 arrays containing all values 0..2 with
 *          every 2 X 2 subblock having one or two distinct values, and new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A210089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 20*a(n-1) - 112*a(n-2) + 122*a(n-3) + 61*a(n-4) -
 *            106*a(n-5) + 16*a(n-6) with initial values a(1) = 9, a(2) = 169,
 *            a(3) = 2370, a(4) = 29650, a(5) = 348621, a(6) = 3950131
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed for dynamic allocation */
#include <stdlib.h>

/* Compute the n-th term of OEIS sequence A210089.
   n is 1‑based and n >= 1. */
seq_t a(idx_t n) {
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;        /* array of terms, 1‑based */
    static idx_t cache_last = 0;       /* highest index computed so far */
    static idx_t cache_cap = 0;        /* allocated size (cache_cap > 0) */

    /* Initial values (index 1..6).  cache[0] is unused. */
    const seq_t init[6] = { 9, 169, 2370, 29650, 348621, 3950131 };

    /* Ensure that cache is allocated and contains the 6 initials. */
    if (cache == NULL) {
        /* Allocate space for indices 0..6 (cache_cap = 7). */
        cache_cap = 7;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure, return 0 */
        cache[0] = 0;
        for (idx_t i = 1; i <= 6; ++i)
            cache[i] = init[i - 1];
        cache_last = 6;
    } else if (cache_last < 6) {
        /* If cache was smaller than needed (unlikely), fill missing initials. */
        for (idx_t i = cache_last + 1; i <= 6; ++i)
            cache[i] = init[i - 1];
        cache_last = 6;
    }

    /* If requested term already computed, return it. */
    if (n <= cache_last)
        return cache[n];

    /* Grow cache (doubling strategy) until it can hold index n. */
    while (cache_cap <= n) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 7;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms from cache_last+1 up to n using the recurrence. */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* The recurrence requires indices i-1 .. i-6, which are already computed. */
        seq_t term1 = 20 * cache[i - 1]
                    + 122 * cache[i - 3]
                    + 61  * cache[i - 4]
                    + 16  * cache[i - 6];
        seq_t term2 = 112 * cache[i - 2]
                    + 106 * cache[i - 5];
        cache[i] = term1 - term2;      /* safe: term1 >= term2 for all valid i */
    }

    cache_last = n;
    return cache[n];
}
