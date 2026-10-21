/*
 * A215153  Primes congruent to {1, 2, 3} mod 17.
 * https://oeis.org/A215153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p mod 17 ∈ {1, 2, 3} (offset 1)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/append_candidate.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1, below is unused */

    /* static cache of sequence values   */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next integer to test for primality (starts at 2)   */
    static uint64_t next_candidate = 2;

    /* if we already have the term, return it      */
    if ((idx_t)cache_len >= n)
        return cache[n - 1];

    /* extend cache until it covers the requested index   */
    while ((idx_t)cache_len < n)
    {
        /* test next_candidate for primality
           For small numbers like those we tackle, isPrimeGeneric
           is fast enough.                                   */
        if (isPrimeGeneric((seq_t)next_candidate))
        {
            uint64_t mod = next_candidate % 17U;
            if (mod == 1U || mod == 2U || mod == 3U)
                append_candidate(&cache, &cache_len, &cache_cap,
                                 (seq_t)next_candidate);
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
