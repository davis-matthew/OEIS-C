/*
 * A017868  Expansion of 1/(1-x^8-x^9-x^10).
 * https://oeis.org/A017868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) + a(n-9) + a(n-10) for n > 9 Initial values: a(0)
 *            = 1, a(1…7) = 0, a(8) = 1, a(9) = 1, a(10) = 1
 * Range:     u8 n <= 60, u16 n <= 106, u32 n <= 198, u64 n <= 379,
 *            u128 n <= 741, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size (number of elements) */
    static size_t last_computed = 0;   /* highest index that has a valid value */

    /* Convert index to size_t for temporary calculations */
    size_t N = (size_t)n;

    /* Initialise cache on first call */
    if (!cache) {
        /* Allocate space for indices 0..10 (the initial values) */
        cache_size = 11;
        cache = (seq_t*)calloc(cache_size, sizeof(seq_t));
        if (!cache) abort(); /* unlikely but keep the program safe */

        /* Base values */
        cache[0] = 1;       /* a(0) = 1 */
        /* a(1)..a(7) are already zero from calloc */
        cache[8] = 1;       /* a(8) = 1 */
        cache[9] = 1;       /* a(9) = 1 */
        cache[10] = 1;      /* a(10) = 1 */

        last_computed = 10;
    }

    /* Extend cache if requested N is beyond current capacity */
    if (N >= cache_size) {
        size_t new_size = N + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        /* Initialise newly allocated slots to zero */
        for (size_t i = cache_size; i < new_size; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute values up to N if we haven't already */
    if (N > last_computed) {
        for (size_t i = last_computed + 1; i <= N; ++i) {
            if (i <= 10) {
                /* values 0‑10 are already set */
                continue;
            }
            /* recurrence for i > 10 */
            cache[i] = cache[i - 8] + cache[i - 9] + cache[i - 10];
        }
        last_computed = N;
    }

    return cache[N];
}
