/*
 * A103401  Numbers k such that k211 is prime.
 * https://oeis.org/A103401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that 1000*k+211 is prime */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h" /* provides bool is_prime(uint64_t) */

seq_t a(idx_t n)
{
    /* Keep a cache of the first m values found so far.
       The cache grows as needed.  All accesses are 1‑based
       to match the OEIS offset. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements already stored */
    static idx_t cache_cap = 0;   /* allocated capacity */
    static seq_t last_k = 0;      /* last k that was added to the cache */

    if (n == 0)  /* should never happen – offset is 1 */
        return 0;

    /* generate values until we have at least n of them */
    while (cache_len < n)
    {
        /* Expand cache if necessary */
        if (cache_len == cache_cap)
        {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
            {
                /* allocation failure – abort silently (undefined behavior) */
                abort();
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Candidate k to test */
        seq_t k = (cache_len == 0) ? 0 : last_k + 1;
        last_k = k;

        /* Form the number 1000*k + 211 and test primality.
           The value fits into 64 bits for all feasible n. */
        uint64_t candidate = 211ULL + 1000ULL * (uint64_t)k;
        if (is_prime(candidate))
        {
            cache[cache_len++] = k; /* store k in the cache */
        }
    }

    return cache[n - 1];
}
