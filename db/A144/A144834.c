/*
 * A144834  Numbers n such that the two numbers n+1 and n+3 are both prime.
 * https://oeis.org/A144834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 860, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A144834: numbers n such that n+1 and n+3 are both prime
 * (i.e. n = p−1 for each twin prime pair p, p+2).
 * The function generates twin primes on demand and caches results.
 * It works for all idx_t >= 1 and is independent of call order.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;     /* number of terms stored */
    static idx_t cache_cap = 0;     /* allocated capacity of cache */
    static idx_t next_candidate = 2;/* next n to test */

    /* Return cached value if available */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow cache as needed */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Find twin primes until we have at least n terms */
    while (cache_len < n) {
        /* Test if next_candidate + 1 and + 3 are both prime */
        if (isPrimeGeneric((seq_t)(next_candidate + 1)) &&
            isPrimeGeneric((seq_t)(next_candidate + 3))) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
