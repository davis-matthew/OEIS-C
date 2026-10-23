/*
 * A144769  a(n) = floor(prime(n)/3).
 * https://oeis.org/A144769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(prime(n)/3)
 * Range:     u8 n <= 54, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Compute a(n) = floor(prime(n)/3).  The function caches primes
   that have been found so far.  It never assumes that calls arrive
   in increasing order of n. */
seq_t a(idx_t n)
{
    /* Static cache of primes.  We store them as 32‑bit values
       because the required primes are all < 2^32 for the ranges
       we care about (n up to about 10000). */
    static uint32_t *prime_cache = NULL;
    static size_t prime_cache_cap = 0;  /* allocated capacity */
    static idx_t  prime_cache_cnt = 0;  /* number of primes stored */

    /* If we already have enough primes, use the cache directly. */
    if (prime_cache_cnt < n) {
        /* Find primes until we have at least n of them. */
        idx_t candidate = (prime_cache_cnt == 0) ? 2 : (idx_t)(prime_cache[prime_cache_cnt - 1] + 1);
        while (prime_cache_cnt < n) {
            /* Look for the next prime. */
            while (!is_prime(candidate))
                ++candidate;
            /* Grow the cache if necessary. */
            if (prime_cache_cnt == prime_cache_cap) {
                size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 128;
                uint32_t *tmp = (uint32_t *)realloc(prime_cache, new_cap * sizeof(uint32_t));
                if (!tmp)         /* allocation failed */
                    return 0;
                prime_cache = tmp;
                prime_cache_cap = new_cap;
            }
            prime_cache[prime_cache_cnt++] = (uint32_t)candidate;
            ++candidate;
        }
    }

    /* Compute floor(prime(n)/3).  The prime is stored in a 32‑bit
       unsigned, so the division is performed in the seq_t type. */
    seq_t p = (seq_t)prime_cache[n - 1];
    return p / (seq_t)3;
}
