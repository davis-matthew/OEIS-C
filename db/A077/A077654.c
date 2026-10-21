/*
 * A077654  Composites k such that 2k+1 is also composite.
 * https://oeis.org/A077654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th composite k such that 2*k+1 is also composite
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* static cache: we keep the sequence as it's generated, so
       subsequent calls can reuse it even if the indices are out of
       order. */
    static seq_t *cache     = NULL;   /* array of found terms */
    static size_t cache_len = 0;      /* number of terms stored */
    static size_t cache_cap = 0;      /* capacity of the array */
    static idx_t  last_k    = 3;      /* last k examined (we start from 4) */

    /* Generate terms until we have at least n terms in the cache. */
    while (cache_len < n)
    {
        ++last_k;
        seq_t k = (seq_t)last_k;              /* candidate k */
        if (is_prime_small(k))
            continue;                         /* skip primes */

        seq_t two_k_plus_one = k * 2 + 1;
        if (is_prime_small(two_k_plus_one))
            continue;                         /* skip if 2k+1 is prime */

        /* append to cache */
        if (cache_len == cache_cap)
        {
            cache_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp)
                abort();   /* memory allocation failed */
            cache = tmp;
        }
        cache[cache_len++] = k;
    }

    return cache[n - 1];
}
