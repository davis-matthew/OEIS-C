/*
 * A238243  A recursive sequence: a(n) = Fibonacci(n)*a(n-1) + 2.
 * https://oeis.org/A238243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n) * a(n-1) + 2 , a(1) = 1 (recurrence)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   ensure_cache, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/fib.h"      /* provides: seq_t fib(idx_t n)      */
#include "../utils/ensure_cache.h" /* provides: void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 we return 0. */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;     /* cache[i] = a(i) */
    static size_t cache_cap = 0;    /* current capacity of cache */
    static idx_t cache_max = 0;     /* maximum index with valid value */
    static bool initialized = false;

    /* Initialise the cache for the first term. */
    if (!initialized) {
        /* Ensure that the cache can hold index 1. */
        ensure_cache(1, &cache, &cache_cap);
        cache[1] = (seq_t)1;   /* a(1) = 1 */
        cache_max = 1;
        initialized = true;
    }

    /* Expand cache if necessary. */
    if (n > cache_max) {
        ensure_cache(n, &cache, &cache_cap);

        for (idx_t k = cache_max + 1; k <= n; ++k) {
            /* a(k) = Fib(k) * a(k-1) + 2 */
            seq_t fk   = fib(k);          /* Fibonacci(k) */
            seq_t prev = cache[k - 1];    /* a(k-1) */
            seq_t val  = fk * prev + (seq_t)2;
            cache[k] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
