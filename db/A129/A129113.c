/*
 * A129113  Expansion of x^3 / (1 - x - 5*x^2 - x^3 + x^4).
 * https://oeis.org/A129113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2) + a(n-3) - a(n-4), with a(0)=0, a(1)=0,
 *            a(2)=0, a(3)=1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;           /* capacity of cache  */
    /* Initial values */
    static const seq_t init[4] = {0, 0, 0, 1};

    /* Ensure cache exists and has space for indices up to n */
    if (cache == NULL) {
        /* Start with at least 4 slots (needed for the recurrence) */
        cap = (n + 1 > 4) ? (n + 1) : 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* initialise first four known values */
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        /* Remaining slots are zero-initialised for safety */
        for (idx_t i = 4; i < cap; ++i)
            cache[i] = 0;
    } else if (n + 1 > cap) {
        /* Need more space – grow (double or enough for n) */
        idx_t new_cap = cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return 0 as a safe fallback */
            return 0;
        }
        cache = tmp;
        /* Set any new indices to 0 before computing */
        for (idx_t i = cap; i < new_cap; ++i)
            cache[i] = 0;
        /* Compute new values using the recurrence */
        for (idx_t i = cap; i < new_cap; ++i) {
            cache[i] = cache[i-1] + 5 * cache[i-2] + cache[i-3] - cache[i-4];
        }
        cap = new_cap;
    }

    /* If the requested index is below the known initial values,
       it is already correctly stored in the cache. */
    return cache[n];
}
