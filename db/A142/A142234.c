/*
 * A142234  Primes congruent to 37 mod 41.
 * https://oeis.org/A142234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = the n-th prime congruent to 37 (mod 41) by successive search. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;   /* array of found primes */
    static idx_t  cache_len = 0;  /* number of primes stored */

    if (n < 1) return 0;          /* undefined for offset 1, but safe */

    /* Ensure we have at least n primes cached */
    if (cache_len < n) {
        /* Reallocate cache to hold at least n primes */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        if (!new_cache) return 0;  /* allocation failed */
        cache = new_cache;

        /* Start searching from the next candidate after the last cached prime */
        seq_t candidate = (cache_len == 0) ? 37 : cache[cache_len - 1] + 41;

        /* Fill cache until we reach the desired n */
        while (cache_len < n) {
            if (is_prime((idx_t)candidate)) {
                cache[cache_len++] = candidate;
            }
            candidate += 41; /* next candidate in the AP 37 mod 41 */
        }
    }

    /* Return the requested value (1‑based index) */
    return cache[(size_t)(n - 1)];
}
