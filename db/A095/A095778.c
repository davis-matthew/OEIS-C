/*
 * A095778  Values of n for which A095777(n) is 9 (those terms which are
 *          expressible in decimal digits for bases 2 through 10, but not for
 *          base 11).
 * https://oeis.org/A095778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the nth positive integer whose representation in base 11
 *            * contains at least one digit 10 (the digit “A”). Implementation
 *            simply * scans the natural numbers, keeps a cache of found
 *            terms, and returns the * desired term. The helper `has_digit10`
 *            tests whether a value in base‑11 * contains...
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_digit10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_digit10.h"

seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache      = NULL;   /* cached values */
    static idx_t  cache_len = 0;        /* number of cached values */
    static idx_t  cache_cap = 0;        /* allocated capacity */
    static seq_t  last_val   = 0;       /* last positive integer examined */

    /* Quick answer if the requested index is already cached. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Initialise the cache on the first call. */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache)
            return 0; /* allocation failure – unlikely in the test harness */
    }

    /* Continue scanning numbers until enough terms are cached. */
    while (cache_len < n) {
        seq_t val = last_val + 1;
        ++last_val;

        if (has_digit10(val)) {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                if (!tmp)            /* allocation failure */
                    return 0;
                cache = tmp;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];
}
