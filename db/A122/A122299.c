/*
 * A122299  Expansion of x * (1-x) / ( 1 - 2*x - 3*x^2 + x^3 ).
 * https://oeis.org/A122299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A122299
 *  Recurrence:
 *      a(1) = 0
 *      a(2) = 1
 *      a(3) = 1
 *      a(n) = 2*a(n-1) + 3*a(n-2) - a(n-3)   for n ≥ 4
 *
 *  The function memoizes the sequence so that calls
 *  arrive in any order and re-use already computed values.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices below the offset – behaviour is irrelevant. */
    if (n < 1)
        return 0;

    static seq_t *cache = NULL;      /* array of computed values, indices 0..cached_to */
    static idx_t cached_to = 0;      /* highest index currently stored */

    /* If the cache is empty, initialise it. */
    if (cache == NULL) {
        /* Allocate room for indices 0..n */
        size_t size = (size_t)n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!cache)
            return 0; /* Allocation failed – return 0 as a safe fallback */

        /* Initialise all entries to 0 (default). */
        for (size_t i = 0; i < size; ++i)
            cache[i] = 0;

        /* Set the three initial values. */
        if (n >= 1) cache[1] = 0;
        if (n >= 2) cache[2] = 1;
        if (n >= 3) cache[3] = 1;

        cached_to = n;

        /* Compute values from 4 up to n. */
        for (idx_t i = 4; (idx_t)i <= n; ++i) {
            cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] - cache[i - 3];
        }
        return cache[n];
    }

    /* If we already have the requested value, return it. */
    if (n <= cached_to)
        return cache[n];

    /* Extend the cache if necessary. */
    size_t new_size = (size_t)(n + 1);
    if (new_size > (size_t)cached_to + 1) {
        size_t grow_to = new_size * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * grow_to);
        if (!new_cache)
            return 0; /* Realloc failed – safe fallback */
        cache = new_cache;
    }

    /* Compute any missing values. */
    for (idx_t i = cached_to + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 0;
        } else if (i == 2) {
            cache[2] = 1;
        } else if (i == 3) {
            cache[3] = 1;
        } else {
            cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] - cache[i - 3];
        }
    }

    cached_to = n;
    return cache[n];
}
