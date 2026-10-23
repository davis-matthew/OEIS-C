/*
 * A167443  Prime numbers ending in the prime number 41.
 * https://oeis.org/A167443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 157, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167443: nth prime ending in 41 (primes ≡ 41 (mod 100)) */
/* The function searches successive numbers of the form 100k+41, tests
   primality with is_prime(idx_t), and memoizes the results. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* is_prime(idx_t) */
#include "../utils/seq_alloc.h"     /* seq_alloc(size_t) */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*, size_t, size_t) */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    static seq_t *cache = NULL;      /* cached primes */
    static size_t cache_size = 0;    /* number of cached primes */
    static size_t cache_cap  = 0;    /* capacity of cache array */
    static idx_t  candidate  = 41;   /* next number of the form 100k+41 */

    /* Convert 1-based index to 0-based target */
    size_t target = (size_t)n - 1;

    /* Initialise cache if needed */
    if (!cache) {
        cache_cap = 64;                     /* initial capacity */
        cache     = seq_alloc(cache_cap);   /* allocate array of seq_t */
    }

    /* Generate primes until we have at least (target+1) primes */
    while (cache_size <= target) {
        if (is_prime(candidate)) {
            /* Grow cache if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
                cache = new_cache;
                cache_cap = new_cap;
            }
            /* Store the prime (casting is safe for values that fit in seq_t) */
            cache[cache_size++] = (seq_t)candidate;
        }
        candidate += 100;    /* move to next number ending with 41 */
    }

    return cache[target];
}
