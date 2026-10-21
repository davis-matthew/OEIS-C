/*
 * A237814  Primes p such that 2*p+1 and 2*p+19 are also prime.
 * https://oeis.org/A237814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 106, u32 n <= 107, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime p such that 2*p+1 and 2*p+19 are also prime. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 0;   /* start searching from 1 */

    if (n < 1) return 0;               /* undefined for zero or negative indices */

    /* Build cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Find the next prime after last_checked */
        uint64_t p = next_prime(last_checked + 1);
        last_checked = p;        /* advance the search frontier regardless of validity */

        /* Check companion primes in seq_t domain to avoid overflow */
        seq_t q1 = ((seq_t)p) * 2 + 1;
        seq_t q2 = ((seq_t)p) * 2 + 19;
        if (is_prime_big(q1) && is_prime_big(q2)) {
            /* Grow cache when needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    /* Return the requested term (1‑based indexing) */
    return cache[n - 1];
}
