/*
 * A142407  Primes congruent to 29 mod 48.
 * https://oeis.org/A142407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth prime congruent to 29 (mod 48)
 * Range:     u8 n <= 2, u16 n <= 403, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* outside the defined offset */

    /* Static cache to avoid recomputing primes for repeated calls */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;   /* number of primes stored in cache */
    static idx_t  next_k = 0;  /* k for the next candidate 48*k+29 */

    /* Return already computed value if available */
    if (n <= len)
        return cache[n - 1];

    /* Initialise or grow the cache storage */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate primes until we have at least n of them */
    while (len < n) {
        idx_t cand = 48 * next_k + 29;   /* numbers ≡ 29 (mod 48) */
        if (is_prime(cand)) {
            if (len == cap) {          /* double the storage if needed */
                cap *= 2;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = (seq_t)cand; /* store the prime as seq_t */
        }
        ++next_k;
    }

    return cache[n - 1];
}
