/*
 * A059111  a(n) = floor(prime(n) - n*log(n)).
 * https://oeis.org/A059111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(prime(n) - n*log(n))
 * Range:     u8 n <= 283, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ceil_nlog
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/ceil_nlog.h"

/* Global cache of the first primes found so far */
static idx_t *prime_cache    = NULL; /* array of primes */
static size_t prime_cache_size  = 0; /* allocated capacity */
static size_t prime_cache_count = 0; /* number of primes stored */

/* Ensure that the cache contains at least the first n primes. */
static void ensure_prime_cache(idx_t n)
{
    if (prime_cache_count >= (size_t)n) return;

    /* Expand capacity if needed. */
    if (prime_cache_count >= prime_cache_size) {
        prime_cache_size = prime_cache_size ? prime_cache_size * 2 : 16;
        prime_cache = (idx_t *)realloc(prime_cache,
                                       prime_cache_size * sizeof(idx_t));
    }

    /* Generate additional primes until we have n of them. */
    while (prime_cache_count < (size_t)n) {
        idx_t candidate = (prime_cache_count == 0) ? 2
                                           : prime_cache[prime_cache_count - 1] + 1;

        for (;;) {
            bool is_prime = true;
            /* Trial division by the primes we already have. */
            for (size_t i = 0; i < prime_cache_count; ++i) {
                idx_t p = prime_cache[i];
                /* Stop if p*p > candidate. */
                if ((uint64_t)p * (uint64_t)p > (uint64_t)candidate) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }

            if (is_prime) {
                /* Store the new prime. */
                prime_cache[prime_cache_count++] = candidate;

                /* Make sure there's still room. */
                if (prime_cache_count >= prime_cache_size) {
                    prime_cache_size *= 2;
                    prime_cache = (idx_t *)realloc(prime_cache,
                                                   prime_cache_size * sizeof(idx_t));
                }
                break;  /* next prime found */
            }
            ++candidate;
        }
    }
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* unused, offset is 1 */

    /* Make sure we have the n-th prime. */
    ensure_prime_cache(n);
    idx_t pn = prime_cache[n - 1];          /* 0‑based index */

    seq_t ceil_val = ceil_nlog(n);          /* ceil(n*log(n))  */
    return (seq_t)pn - ceil_val;
}
