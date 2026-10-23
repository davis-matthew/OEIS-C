/*
 * A142750  Primes congruent to 23 mod 59.
 * https://oeis.org/A142750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 23 (mod 59)
 * Range:     u8 n <= 1, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t  *cache   = NULL;   /* array of found primes          */
static size_t  cache_len = 0;    /* number of stored primes        */
static size_t  cache_cap = 0;    /* current capacity of the array  */
static idx_t   next_candidate = 23; /* next 59*k+23 to test            */
static bool    initialized   = false;

/* Ensure the cache can hold at least 'needed' primes. */
static void grow_cache(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation error */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* External interface – return the n-th prime ≡23 (mod 59). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined below offset */

    /* Lazy initialization of the cache */
    if (!initialized) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache_len = 0;
        next_candidate = 23;
        initialized = true;
    }

    /* Generate primes until we have at least n of them */
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 59;
    }

    return cache[n - 1];
}
