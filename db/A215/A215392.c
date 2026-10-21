/*
 * A215392  Primes congruent to {1, 2} mod 17.
 * https://oeis.org/A215392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p > 0 with p % 17 ∈ {1, 2}
 * Range:     u8 n <= 7, u16 n <= 824, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple incremental sieve with memoisation.
   It keeps a static cache of primes that satisfy the congruence
   condition.  On the first call it starts testing numbers from 2
   upwards, and only adds a number to the cache when it is prime and
   congruent to 1 or 2 modulo 17.  Subsequent calls reuse the cache
   without recomputation.  The cache grows as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Static cache of qualified primes */
static seq_t *prime_cache = NULL;
static size_t cache_count = 0;  /* number of primes stored */
static size_t cache_capacity = 0;  /* current allocated capacity */

/* The next candidate number to test */
static seq_t next_candidate = 2;

/* Return the n-th prime congruent to 1 or 2 modulo 17 */
seq_t a(idx_t n)
{
    /* Access to cache may need the target number of primes */
    size_t target = (size_t)n;   /* n fits in size_t because n is small */

    /* Ensure the cache contains at least 'target' primes */
    while (cache_count < target) {
        /* Test only if candidate ≡ 1 or 2 (mod 17) */
        seq_t rem = next_candidate % 17;
        if (rem == 1 || rem == 2) {
            /* If prime, store it */
            if (isPrimeGeneric(next_candidate)) {
                /* Grow cache if necessary */
                if (cache_count == cache_capacity) {
                    size_t new_cap = cache_capacity ? (cache_capacity << 1) : 8;
                    seq_t *tmp = (seq_t *)realloc(prime_cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();   /* unrecoverable allocation failure */
                    prime_cache = tmp;
                    cache_capacity = new_cap;
                }
                prime_cache[cache_count++] = next_candidate;
            }
        }
        /* Advance to next integer */
        next_candidate++;
    }

    /* Return the requested prime (1‑based index) */
    return prime_cache[(size_t)(n - 1)];
}
