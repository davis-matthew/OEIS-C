/*
 * A215378  Primes congruent to {0, 1, 2, 3} mod 11.
 * https://oeis.org/A215378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p mod 11 ∈ {0,1,2,3}
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (primes 2,3,11,13,23,...) as listed in OEIS A215378 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/append_candidate.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Cache of computed terms */
    static seq_t *cache = NULL;      /* array of primes satisfying the condition */
    static size_t cache_len = 0;     /* number of cached terms */
    static size_t cache_cap = 0;     /* capacity of the cache array */
    static seq_t current = 2;        /* next integer to test for primality */

    if (n == 0) return 0;           /* undefined for offset 0 */

    /* Extend cache until we have at least n terms */
    while (cache_len < (size_t)n)
    {
        if (is_prime_small((uint64_t)current) &&
            ((uint64_t)current % 11U) <= 3U)
        {
            /* store the term in the cache */
            append_candidate(&cache, &cache_len, &cache_cap, current);
        }
        /* advance to the next candidate */
        current++;
    }

    return cache[(size_t)(n - 1)];
}
