/*
 * A124849  Numbers k such that 7k + 3 and 3k + 7 are primes.
 * https://oeis.org/A124849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A124849: numbers k such that 7 k + 3 and 3 k + 7 are primes. */
/* Implementation: simple incremental sieve with a static cache. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache          = NULL;
    static size_t cache_len      = 0;
    static size_t cache_cap      = 0;
    static idx_t  next_k         = 0;    /* next candidate k to test */

    while (cache_len < n)
    {
        idx_t k      = next_k;
        idx_t p1     = 7 * k + 3;        /* 7 k + 3 */
        idx_t p2     = 3 * k + 7;        /* 3 k + 7 */

        if (is_prime(p1) && is_prime(p2))
        {
            /* grow cache if needed */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                /* C++ requires a cast from void* */
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
