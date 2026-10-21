/*
 * A073085  Numbers k such that 210*k+1 is prime.
 * https://oeis.org/A073085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that 210*k+1 is prime, with a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=5, …, enumerated in increasing k
 * Range:     u8 n <= 1, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Simple cache to avoid re‑searching the same values on subsequent calls. */
    static seq_t *cache         = NULL;
    static idx_t cache_size     = 0;
    static idx_t cache_cap      = 0;
    static idx_t last_k_checked = 0;    /* last k examined by the search */

    /* initialise cache on the first call */
    if (!cache) {
        cache_cap = 128;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* extend the cache until we have at least the requested term */
    while (cache_size < n) {
        /* next candidate k */
        last_k_checked++;

        /* 210*k + 1, computed in 64‑bit to stay within the "
           bound for primes we are testing. */
        uint64_t num = 210ULL * (uint64_t)last_k_checked + 1ULL;

        /* primality test using the small‑int helper */
        if (is_prime_small(num)) {
            /* ensure room in the cache */
            if (cache_size + 1 > cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache,
                                                     sizeof(seq_t) * new_cap);
                if (!new_cache) abort();   /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            /* store the k value */
            cache[cache_size++] = (seq_t)last_k_checked;
        }
    }

    return cache[n - 1];
}
