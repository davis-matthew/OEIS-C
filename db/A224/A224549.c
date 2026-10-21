/*
 * A224549  Number of (n+1) X 8 0..1 matrices with each 2 X 2 subblock
 *          idempotent.
 * https://oeis.org/A224549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5) for n>6
 *            Initial values: a(1)=216, a(2)=345, a(3)=472, a(4)=682,
 *            a(5)=943, a(6)=1323
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 38, u64 n <= 84, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types supplied by the build system
 * typedef unsigned long long idx_t;
 * typedef unsigned long long seq_t;
 */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms.
       cache[0] is unused; indices 1..last_computed contain valid values. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = 0;

    /* Initialise cache with the first six terms on first call. */
    if (cache == NULL) {
        cap = 7;                     /* indices 0..6 inclusive */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL)            /* allocation failure – should never happen */
            return (seq_t)0;

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)216;
        cache[2] = (seq_t)345;
        cache[3] = (seq_t)472;
        cache[4] = (seq_t)682;
        cache[5] = (seq_t)943;
        cache[6] = (seq_t)1323;
        last_computed = 6;
    }

    /* If the requested term is already in cache, return it. */
    if (n <= last_computed && n >= 1)
        return cache[n];

    /* Expand cache if necessary. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL)          /* reallocation failure – should never happen */
            return (seq_t)0;
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute missing values up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i - 1]
                 - 5 * cache[i - 2]
                 +   cache[i - 3]
                 + 2 * cache[i - 4]
                 -   cache[i - 5];
    }
    last_computed = n;

    return cache[n];
}
