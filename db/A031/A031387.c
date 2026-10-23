/*
 * A031387  a(n) = prime(6*n-3).
 * https://oeis.org/A031387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031387: a(n) = prime(6*n-3)  (1‑based index) */
/* The function returns the (6*n-3)-th prime for n >= 1.  It caches
   primes across calls so that non‑monotonic n are handled efficiently. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices below the OEIS offset (1). */
    if (n < 1)
        return (seq_t)0;

    /* Compute the required prime index k = 6*n-3. */
    idx_t k = 6 * n - 3;
    if (k < 1)
        return (seq_t)0;   /* safety check, but k should always be >=4 */

    /* Static cache of the first `prime_cache_len` primes. */
    static uint32_t *prime_cache = NULL;
    static size_t prime_cache_len = 0;

    /* If we need more primes than we have cached, regenerate the cache. */
    if ((size_t)k > prime_cache_len) {
        uint32_t *new_cache = NULL;
        size_t new_len = 0;
        /* The helper allocates the array; we take ownership. */
        sieve_first_n_primes(k, &new_cache, &new_len);
        /* Replace the old cache (if any). */
        free(prime_cache);
        prime_cache = new_cache;
        prime_cache_len = new_len;
    }

    /* Return the k-th prime (1‑based indexing).  The prime fits in
       uint32_t, so a cast to seq_t is safe for all supported builds
       because seq_t is an unsigned integer type of equal or greater
       width. */
    return (seq_t)prime_cache[k - 1];
}
