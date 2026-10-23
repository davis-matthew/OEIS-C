/*
 * A167281  Primes containing the string 555.
 * https://oeis.org/A167281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 12, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   contains_555, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A167281: primes containing the decimal substring "555"
/* This implementation searches for primes that contain the string
   "555", memoises the results, and returns the n‑th such prime.
   It satisfies the requirements for both C11 and C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */
#include "../utils/contains_555.h"

/* Static cache for already computed terms.  The cache is
   grown lazily as needed.  The size is limited so that the
   function is fast even when called out of order. */
static seq_t *cache = NULL;   /* dynamically allocated array of seq_t */
static size_t cache_len = 0;  /* number of values actually stored   */
static size_t cache_cap = 0;  /* allocated capacity                 */

/* Compute the n‑th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; guard against zero */
        return 0;

    /* Return cached value if already computed */
    if (cache_len >= (size_t)n)
        return cache[(size_t)n - 1];

    /* Start searching from the last found prime + 1,
       or from the smallest prime if the cache is empty. */
    uint64_t current = (cache_len == 0) ? 2ULL
                                        : (uint64_t)cache[cache_len - 1] + 1ULL;

    while (cache_len < (size_t)n) {
        /* Skip numbers that do not contain the substring 555 */
        while (!contains_555(current))
            ++current;

        /* Test primality using the generic big‑primality routine. */
        if (is_prime_big((seq_t)current)) {
            /* Grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) /* allocation failed */
                    exit(EXIT_FAILURE);
                cache      = new_cache;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = (seq_t)current;
        }
        ++current;
    }

    return cache[(size_t)n - 1];
}
