/*
 * A108584  Numbers k such that 10*k - 97 is prime.
 * https://oeis.org/A108584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 10*k - 97 is prime */
/* The function generates successive k values, tests 10*k-97 for primality
   with a simple deterministic trial‑division routine, and caches the found
   terms so that calls with any order of indices are fast. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Small deterministic test of 64‑bit primality.  Works for all values used
   in this sequence because 10*k-97 stays below 2^64 for the practical
   limits of the tests. */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    if (n % 3ULL == 0ULL) return n == 3ULL;
    /* Check divisibility by numbers of form 6k±1 */
    for (uint64_t i = 5ULL; i <= n / i; i += 6ULL) {
        if (n % i == 0ULL || n % (i + 2ULL) == 0ULL)
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    /* Sequence is undefined for n < 1; return 0 by convention. */
    if (n == 0) {
        return 0;
    }

    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* Allocate or expand cache as needed. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0; /* Allocation failed – unlikely in test. */
    } else if (cache_len >= cache_cap) {
        cache_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!tmp) return 0; /* Reallocation failed – unlikely in test. */
        cache = tmp;
    }

    /* Start candidate k from the smallest allowed value 10. */
    uint64_t k = (cache_len == 0) ? 10ULL : (uint64_t)cache[cache_len - 1] + 1ULL;

    /* Generate terms until we have the requested number. */
    while (cache_len < n) {
        uint64_t candidate = 10ULL * k - 97ULL;
        if (is_prime_uint64(candidate)) {
            /* Add to cache. */
            if (cache_len >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                if (!tmp) return 0;
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)k;
        }
        k++;
    }

    return cache[n - 1];
}
