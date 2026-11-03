/*
 * A208270  Primes containing a digit 1.
 * https://oeis.org/A208270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th prime that contains the digit '1' (searched)
 * Range:     u8 n <= 32, u16 n <= 3559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9918 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* nth prime containing the digit '1' */
seq_t a(idx_t n)
{
    /* n is 1-based. Behaviour for n < 1 is undefined, return 0. */
    if (n < 1U) return 0;

    /* Static cache to store terms that have been found. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static uint64_t last_candidate = 1;   /* last tested integer */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Find next prime that has digit 1. */
        for (;;) {
            ++last_candidate;
            if (is_prime(last_candidate)) {
                uint64_t t = last_candidate;
                while (t > 0 && t % 10 != 1U) t /= 10U;
                if (t % 10U == 1U) break;  /* found suitable prime */
            }
        }
        /* Store it in the cache. */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_size++] = (seq_t)last_candidate;
    }

    return cache[n - 1];
}
