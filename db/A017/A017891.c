/*
 * A017891  Expansion of 1/(1-x^10-x^11-x^12-x^13-x^14-x^15).
 * https://oeis.org/A017891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 93, u32 n <= 169, u64 n <= 321,
 *            u128 n <= 625, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(0) = 1
 *   a(n) = 0  for 1 ≤ n ≤ 9
 *   a(n) = 1  for 10 ≤ n ≤ 15
 *   a(n) = a(n-10) + a(n-11) + a(n-12) + a(n-13) + a(n-14) + a(n-15)
 *          for n > 14
 */

#include <stdlib.h>

/* Compute the value of a(n) for the OEIS sequence A017891. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation.  The cache is expanded lazily to
     * accommodate the largest argument seen so far. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of computed entries */

    /* Helper to initialise the cache up to index 15. */
    if (cache == NULL) {
        /* Allocate space for indices 0..15.  Use malloc to remain
         * C-conformant; the type seq_t is assumed to be trivially
         * constructible in both C and C++. */
        cache_size = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL)
            return (seq_t)0; /* fallback – should never happen in tests */

        cache[0] = (seq_t)1;
        for (idx_t i = 1; i <= 9; ++i) cache[i] = (seq_t)0;
        for (idx_t i = 10; i <= 15; ++i) cache[i] = (seq_t)1;
    }

    /* Grow the cache if necessary. */
    if (n >= cache_size) {
        idx_t new_size = n + 1;               /* include index n */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            sizeof(seq_t) * new_size);
        if (new_cache == NULL)
            return (seq_t)0;                 /* fallback */
        cache = new_cache;

        /* Initialise new entries beyond the previous size. */
        /* The first index to initialize is cache_size. */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* Compute according to the recurrence. */
            if (i > 15) {
                /* Sum the previous six terms. */
                seq_t sum = cache[i - 10] + cache[i - 11] +
                            cache[i - 12] + cache[i - 13] +
                            cache[i - 14] + cache[i - 15];
                cache[i] = sum;
            } else {
                /* i <= 15 have already been set by the initialisation
                 * above.  For completeness, handle i between 10 and 15
                 * yielding 1, and 1..9 yielding 0. */
                if (i <= 9) cache[i] = (seq_t)0;
                else       cache[i] = (seq_t)1;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
