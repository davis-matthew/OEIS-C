/*
 * A142227  Primes congruent to 30 mod 41.
 * https://oeis.org/A142227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 30 mod 41
 * Range:     u8 n <= 1, u16 n <= 168, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime number p such that p ≡ 30 (mod 41).
   Uses memoized sieve-like generation: candidates are
   30 + 41*k for k = 0,1,2,...  and tested with the provided
   is_prime function.  The results are cached statically so that
   repeated calls with arbitrary n values are fast.  */

#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;    /* array of found primes (seq_t) */
static size_t cache_cap = 0;         /* capacity of prime_cache */
static idx_t cache_len = 0;          /* number of primes stored */
static idx_t next_candidate = 30;    /* next number ≡30 (mod 41) to test */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* behavior below offset doesn't matter */

    /* Grow cache as needed */
    if (n > cache_len) {
        /* Initial allocation */
        if (!prime_cache) {
            cache_cap = 16;
            prime_cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        }

        /* Generate primes until we have at least n entries */
        while (cache_len < n) {
            /* If the current candidate is less than needed (e.g., on first run),
               initialize it to the first valid number */
            if (next_candidate < 30) next_candidate = 30;
            /* Check if current candidate is prime */
            if (is_prime(next_candidate)) {
                /* Ensure there is room in the cache */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    prime_cache = (seq_t*)realloc(prime_cache,
                                                  cache_cap * sizeof(seq_t));
                }
                /* Store the prime (cast to seq_t, overflow is acceptable if
                   it doesn't fit in seq_t, per the specification) */
                prime_cache[cache_len++] = (seq_t)next_candidate;
            }
            /* Move to the next number ≡30 (mod 41) */
            next_candidate += 41ULL;
        }
    }

    /* Return the (n-1)-th element (since cache is 0-indexed) */
    return prime_cache[n - 1];
}
