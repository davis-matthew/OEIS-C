/*
 * A142417  Primes congruent to 4 mod 49.
 * https://oeis.org/A142417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 131, u32 n <= 144, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime that is ≡ 4 (mod 49).
   The algorithm generates candidates of the form 49*k+4,
   tests them for primality with is_prime_big, and stores them
   in a static array that is grown as needed.  The cache allows
   arbitrary ordering of calls. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"     /* for seq_realloc */

/* static cache for found primes */
static seq_t *prime_cache = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of primes stored */
static uint64_t last_k   = 0;   /* last k value processed (k starts at 0) */

/* Ensure that the cache holds at least n primes. */
static void ensure_cache(size_t n)
{
    if (cache_len >= n)
        return;

    /* Generate primes of the form 49*k+4 until we have n primes */
    while (cache_len < n) {
        uint64_t k = ++last_k;                       /* next candidate index */
        seq_t   candidate = (seq_t)49 * (seq_t)k + 4; /* 49*k+4 */

        if (is_prime_big(candidate)) {
            /* Grow the cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = seq_realloc(prime_cache, cache_cap, new_cap);
                if (!tmp) abort();                    /* allocation failure */
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_len++] = candidate;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1; placeholder */

    ensure_cache((size_t)n);
    return prime_cache[n - 1];
}
