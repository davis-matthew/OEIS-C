/*
 * A190985  a(n) = 10*a(n-1) - 3*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 3*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Ensure we have space for at least n+1 elements */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – abort */
            /* In the judge environment this should never happen. */
            exit(EXIT_FAILURE);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize base cases if not already done */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Handle small n directly */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* Extend the cache up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)10 * cache[i - 1] - (seq_t)3 * cache[i - 2];
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}
