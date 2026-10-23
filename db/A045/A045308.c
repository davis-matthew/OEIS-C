/*
 * A045308  Primes congruent to {2, 3, 4, 5} (mod 7).
 * https://oeis.org/A045308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime congruent to 2, 3, 4 or 5 (mod 7). */
/* The function uses a simple sieve‑like search with a static cache */
/* of primes already found. It relies on the helper function is_prime. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cache of primes that satisfy the congruence condition. */
    static idx_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 2;   /* start from the first prime */

    if (n < 1)                     /* undefined for non‑positive indices */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len)
        return (seq_t)cache[n - 1];

    /* Generate further primes until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Find the next prime that satisfies the modulo condition. */
        for (;;) {
            idx_t p = next_candidate++;
            if ((p % 7 == 2) || (p % 7 == 3) ||
                (p % 7 == 4) || (p % 7 == 5)) {
                if (is_prime(p)) {
                    /* Ensure there is space in the cache. */
                    if (cache_len == cache_cap) {
                        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                        idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
                        if (!tmp) {          /* allocation failed */
                            free(cache);
                            cache = NULL;
                            cache_cap = cache_len = 0;
                            return (seq_t)0; /* graceful degradation */
                        }
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = p;
                    break;  /* ready for the next candidate */
                }
            }
        }
    }

    return (seq_t)cache[n - 1];
}
