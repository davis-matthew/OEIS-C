/*
 * A142283  Primes congruent to 34 mod 43.
 * https://oeis.org/A142283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 34 modulo 43
 * Range:     u8 n <= 1, u16 n <= 155, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* December 2026 */
static seq_t *prime_cache  = NULL;   /* Cached list of primes ≡ 34 (mod 43) */
static idx_t   cache_cap   = 0;      /* Current allocated capacity */
static idx_t   cache_len   = 0;      /* Number of primes stored */
static idx_t   last_k      = 0;      /* Last odd multiplier k used in generation */

/* Ensure that prime_cache contains at least n entries. */
static void ensure_primes(idx_t n)
{
    if (cache_len >= n)
        return;

    if (prime_cache == NULL) {
        cache_cap = 1024;
        prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!prime_cache) abort();          /* Out of memory */
    }

    /* Start generating from the last used k.  For an empty cache
       the first candidate is 34 + 43*1 = 77. */
    if (cache_len == 0)
        last_k = 1;
    else
        ; /* last_k already holds the next odd k to try */

    while (cache_len < n) {
        /* Grow the cache if necessary. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(prime_cache,
                                          new_cap * sizeof(seq_t));
            if (!tmp) abort();
            prime_cache = tmp;
            cache_cap = new_cap;
        }

        idx_t candidate = 34 + 43 * last_k;
        if (is_prime(candidate)) {
            prime_cache[cache_len++] = candidate;
        }
        last_k += 2;   /* Next odd k */
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    /* n is 1-indexed; behaviour for n < 1 is undefined. */
    ensure_primes(n);
    return prime_cache[n - 1];
}
