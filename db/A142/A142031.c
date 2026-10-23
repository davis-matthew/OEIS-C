/*
 * A142031  Primes congruent to 27 mod 31.
 * https://oeis.org/A142031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime ≡ 27 (mod 31)
 * Range:     u8 n <= 2, u16 n <= 223, u32 n <= 88, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Static cache of found primes */
static seq_t *cache      = NULL;
static size_t cacheSize  = 0;   /* number of primes stored */
static size_t cacheCap   = 0;   /* allocated capacity of cache */
static seq_t candidate   = 27;  /* current candidate (will be increased before testing) */

seq_t a(idx_t n)
{
    /* --- initialise cache once ------------------------------------------- */
    if (cacheCap == 0) {
        cacheCap = 64; /* initial capacity */
        cache = (seq_t*)malloc(cacheCap * sizeof(*cache));
    }

    /* --- ensure we have at least n entries in the cache ------------------- */
    size_t target = (size_t)n;      /* n ≥ 1 per the problem statement */
    while (cacheSize < target) {
        candidate += (seq_t)31;     /* next number ≡ 27 (mod 31) */
        if (is_prime_big(candidate)) {
            /* grow cache if necessary */
            if (cacheSize >= cacheCap) {
                size_t newCap = cacheCap * 2;
                seq_t *newCache = (seq_t*)realloc(cache, newCap * sizeof(*cache));
                cache    = newCache;
                cacheCap = newCap;
            }
            cache[cacheSize++] = candidate;
        }
    }

    return cache[target - 1];
}
