/*
 * A045382  Primes congruent to {2, 7} mod 8.
 * https://oeis.org/A045382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1649, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 2 or 7 modulo 8
   (sequence A045382).  Uses incremental primality testing
   and an expanding static cache of the primes found so far. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* cache of found primes          */
static size_t cache_cap = 0;     /* capacity of the cache          */
static size_t cached_count = 0;  /* number of primes stored        */
static seq_t current = 2;        /* last candidate examined */
static bool init = false;        /* whether cache has been primed */

static void ensure_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* permutation is 1‑based; 0 is unused */

    if (!init) {            /* initialise with the first prime 2     */
        ensure_cache(1);
        cache[0] = 2;
        cached_count = 1;
        current = 2;        /* last prime already seen */
        init = true;
    }

    /* Generate primes until we have at least n of them */
    while (cached_count < (size_t)n) {
        /* jump to the next integer that is 2 or 7 mod 8 */
        do {
            current++;
        } while ((current % 8 != 2) && (current % 8 != 7));

        if (is_prime_big(current)) {
            ensure_cache(cached_count + 1);
            cache[cached_count++] = current;
        } else {
            /* not prime – continue looping to find the next candidate */
            continue;
        }
    }

    return cache[(size_t)(n - 1)];
}
