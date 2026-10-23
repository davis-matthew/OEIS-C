/*
 * A142854  Primes congruent to 56 mod 61.
 * https://oeis.org/A142854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p % 61 == 56 (offset=1)
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Return the n‑th prime congruent to 56 (mod 61).
       The results are cached in a static array so that calls may
       arrive in any order.  Prime testing is performed by the
       supplied is_prime_small routine.  The cache grows
       geometrically as needed.  For n ≥ 1 the computation is
       well under the time limit for n up to about 10 000. */
    if (n == 0)
        return 0;    /* undefined for n==0, but safe for callers */

    static seq_t *cache = NULL;      /* array of found primes          */
    static size_t cache_len = 0;     /* number of stored primes        */
    static size_t cache_cap = 0;     /* current capacity of array      */
    static uint64_t next_candidate = 56; /* next number to test (mod 61) */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        /* Find the next prime congruent to 56 (mod 61) */
        uint64_t p = next_candidate;
        while (!is_prime_small((seq_t)p))
            p += 61;                   /* still in the congruence class */

        /* Prepare the next candidate for the following search */
        next_candidate = p + 61;

        /* Ensure room in the cache */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* Store the prime found */
        cache[cache_len++] = (seq_t)p;
    }

    /* Return the requested term (indices are 1‑based) */
    return cache[(size_t)n - 1];
}
