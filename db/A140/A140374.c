/*
 * A140374  Primes of the form 26k + 15.
 * https://oeis.org/A140374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 26k + 15 (OEIS A140374). */
/* The first term corresponds to k = 0 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Prime test for arbitrary‐precision or 64‑bit values. */
/* The function is declared in ../utils/is_prime_big.h. */

seq_t a(idx_t n)
{
    /* Static cache of found primes 26k+15. */
    static seq_t *cache      = NULL;   /* cached values                 */
    static size_t cached_len = 0;      /* number of primes cached       */
    static size_t cache_cap  = 0;      /* capacity of cache array       */
    static idx_t  next_k     = 0;      /* next k to check               */

    /* OEIS offset is 1; return 0 for n==0 (undefined). */
    if (n == 0)
        return (seq_t)0;

    /* Grow cache until we have at least n entries. */
    while (cached_len < (size_t)n)
    {
        /* Candidate prime 26k + 15.  Compute in a larger type first. */
        uint64_t cand64 = 15 + 26 * (uint64_t)next_k;
        next_k++;

        /* Convert to seq_t (possible truncation only when value does not fit). */
        seq_t cand = (seq_t)cand64;

        /* Test primality using the provided helper. */
        if (is_prime_big(cand))
        {
            /* Resize cache if necessary. */
            if (cached_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    return (seq_t)0;   /* allocation failed */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cached_len++] = cand;
        }
    }

    return cache[n - 1];
}
