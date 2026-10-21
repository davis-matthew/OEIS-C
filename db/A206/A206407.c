/*
 * A206407  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having an equal number of clockwise and counterclockwise edge
 *          increases.
 * https://oeis.org/A206407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A206407: Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 * having an equal number of clockwise and counterclockwise edge increases.
 *
 * Recurrence (empirical):
 *   a(n) = 9*a(n-1) - 17*a(n-2) + a(n-3) + 4*a(n-4)   for n >= 5
 * Initial values:
 *   a(1) = 81
 *   a(2) = 423
 *   a(3) = 2457
 *   a(4) = 15087
 *
 * The function computes terms on demand and caches them for future calls.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t cap = 0;          /* size allocated for cache (index 0 unused) */
    static idx_t max_computed = 0; /* maximum n for which cache[n] is valid */
    static int initialized = 0;    /* has the cache been initialized? */

    /* Helper macro to cast idx_t to size_t for array subscripting */
    #define IDX(i) ((size_t)(i))

    /* Initialisation: set base values 1..4 */
    if (!initialized) {
        /* allocate space for indices 0..4 (index 0 unused) */
        idx_t new_cap = 5;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory; in practice unlikely */
        cap = new_cap;
        cache[IDX(1)] = 81;
        cache[IDX(2)] = 423;
        cache[IDX(3)] = 2457;
        cache[IDX(4)] = 15087;
        max_computed = 4;
        initialized = 1;
    }

    /* If requested n is beyond current capacity, resize cache */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory */
        cache = tmp;
        cap = new_cap;
    }

    /* For n <= 4 we already have the value */
    if (n <= 4)
        return cache[IDX(n)];

    /* Compute missing terms up to n using recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[IDX(i)] = 9 * cache[IDX(i-1)]
                      - 17 * cache[IDX(i-2)]
                       + cache[IDX(i-3)]
                       + 4 * cache[IDX(i-4)];
    }
    max_computed = n;

    return cache[IDX(n)];
}
