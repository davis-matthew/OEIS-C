/*
 * A141985  Primes congruent to 9 mod 29.
 * https://oeis.org/A141985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is ≡ 9 (mod 29)
 * Range:     u8 n <= 2, u16 n <= 191, u32 n <= 210, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

/* Return the nth term of A141985 – primes ≡ 9 mod 29, with offset 1. */
seq_t a(idx_t n)
{
    /* Static cache of found terms and related state. */
    static seq_t *cache      = NULL;   /* array of found terms           */
    static idx_t cache_len   = 0;       /* number of terms cached         */
    static size_t cache_cap  = 0;       /* memory capacity of cache       */
    static idx_t next_k      = 2;       /* 29*2+9 = 67, first term        */

    /* Keep generating terms until the cache contains at least n. */
    while (cache_len < n)
    {
        /* Candidate number congruent to 9 (mod 29). */
        seq_t candidate = 29 * (seq_t)next_k + (seq_t)9;
        ++next_k;

        /* Test primality of the candidate. */
        if (is_prime_big(candidate))
        {
            /* Allocate cache if this is the first term. */
            if (cache_cap == 0)
            {
                cache_cap = 16;
                cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
                if (!cache) return 0; /* Allocation failed – return 0 as sentinel. */
            }
            /* Grow cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0; /* Realloc failed – return 0. */
                cache    = tmp;
                cache_cap = new_cap;
            }
            /* Store the prime. */
            cache[cache_len++] = candidate;
        }
    }
    /* Return the requested term (offset 1). */
    return cache[n - 1];
}
