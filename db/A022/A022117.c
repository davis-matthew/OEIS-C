/*
 * A022117  Fibonacci sequence beginning 2, 15.
 * https://oeis.org/A022117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 15
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_n = 0;   /* next index that needs to be filled */

    /* initial allocation */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_n = 0;
    }

    /* grow cache if necessary */
    if ((idx_t)n >= (idx_t)cache_cap) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        memcpy(new_cache, cache, cache_n * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise base values if missing */
    if (cache_n == 0) {
        cache[0] = (seq_t)2;
        cache_n = 1;
    }
    if (cache_n == 1) {
        if ((idx_t)n >= 1) {
            cache[1] = (seq_t)15;
            cache_n = 2;
        } else {
            /* n == 0 */
            return cache[0];
        }
    }
    if (cache_n == 0) { /* n == 0 */
        return cache[0];
    }
    if (cache_n == 1) { /* n == 1 */
        return cache[1];
    }

    /* compute missing values up to n */
    for (idx_t i = cache_n; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2];
    }
    cache_n = n + 1;

    return cache[n];
}
