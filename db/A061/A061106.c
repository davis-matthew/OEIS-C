/*
 * A061106  Compute Euler totient function for the prime(n+1)-prime(n)-1
 *          composite numbers between two consecutive primes; choose the
 *          smallest.
 * https://oeis.org/A061106
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Min{phi(c) : prime(n) < c < prime(n+1)}
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_small, phi, prime_limit_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies (provided by the build environment):       */
/*   seq_t, idx_t   – integer types of the build               */
/*   seq_t phi(idx_t m)         – Euler totient                 */
/*   idx_t prime_limit_for_idx(idx_t n) – upper bound for prime(n) */
/*   void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out) */
/*   bool is_prime_small(uint64_t n) – primality test for small n */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/phi.h"
#include "../utils/is_prime_small.h"

/* Cached list of primes (0‑indexed).  This cache is shared by all calls
   to a(), since building primes is expensive.  It is resized only
   when a larger prime index is requested. */
static uint64_t *prime_cache = NULL;
static size_t prime_cache_count = 0;   /* number of primes stored      */
static idx_t   prime_cache_limit = 0;  /* maximum value that was sieved */

/* Ensure that prime_cache contains at least “needed_n_plus” primes,
   i.e. primes up to prime(needed_n_plus).  The function rebuilds the
   sieve only if the current cache is insufficient. */
static void ensure_primes(idx_t needed_n_plus)
{
    /* Get an upper bound on prime(needed_n_plus). */
    idx_t limit = prime_limit_for_idx(needed_n_plus);

    /* Re‑build the sieve if we have no data, or if we were asked for
       more primes than we have, or the requested limit is larger than
       the one that was previously used. */
    if (prime_cache == NULL || prime_cache_count < (size_t)needed_n_plus
        || limit > prime_cache_limit) {

        /* Free the old array if it exists. */
        if (prime_cache != NULL)
            free(prime_cache);
        prime_cache = NULL;
        prime_cache_count = 0;
        prime_cache_limit = limit;

        /* Build a new sieve. */
        build_primes_upto(limit, &prime_cache, &prime_cache_count);

        /* Extremely unlikely: the upper bound may be too small for
           the required number of primes.  Double the bound until it
           suffices. */
        while (prime_cache_count < (size_t)needed_n_plus) {
            limit <<= 1;  /* double the limit */
            if (prime_cache != NULL)
                free(prime_cache);
            prime_cache = NULL;
            prime_cache_count = 0;
            prime_cache_limit = limit;
            build_primes_upto(limit, &prime_cache, &prime_cache_count);
        }
    }
}

/* a(n) = Min{phi(c) : prime(n) < c < prime(n+1)}   for n >= 2.
   For n < 2 the function returns 0 (undefined by OEIS). */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined below the offset */

    /* We need prime(n) and prime(n+1), i.e. the first n+1 primes. */
    ensure_primes(n + 1);

    /* prime_cache is 0‑indexed; prime(n) is at index n-1. */
    uint64_t p_n    = prime_cache[n - 1];
    uint64_t p_next = prime_cache[n];

    seq_t min_phi = 0;  /* 0 indicates “not yet set” */

    for (uint64_t c = p_n + 1; c < p_next; ++c) {
        /* Skip primes: we only consider composites. */
        if (is_prime_small(c))
            continue;

        seq_t phi_val = phi((idx_t)c);
        if (min_phi == 0 || phi_val < min_phi)
            min_phi = phi_val;
    }

    return min_phi;
}
