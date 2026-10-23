/*
 * A142538  Primes congruent to 8 mod 53.
 * https://oeis.org/A142538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 123, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 8 (mod 53) (OEIS A142538) */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of previously found primes congruent to 8 mod 53. */
    static seq_t *cache      = NULL;        /* array of found primes            */
    static size_t cache_size = 0;           /* number of cached entries         */
    static size_t cache_cap  = 0;           /* allocated capacity (in entries)  */
    static idx_t   last_k     = 0;          /* highest k examined (p = 53k+8)   */

    if (n == 0) return 0;   /* behaviour below the offset is irrelevant */

    /* Grow the cache until it contains at least n elements. */
    while (cache_size < n)
    {
        idx_t k = last_k + 1;
        uint64_t cand = 53ULL * k + 8ULL;   /* candidate prime */

        if (is_prime(cand))
        {
            /* Ensure sufficient capacity. */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();          /* out‑of‑memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size] = (seq_t)cand;
            cache_size++;
        }
        last_k = k;
    }
    return cache[n - 1];   /* return the n-th prime ≡ 8 (mod 53) */
}
