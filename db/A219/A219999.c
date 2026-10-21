/*
 * A219999  Decades whose prime pattern is the same as prime pattern in the
 *          previous decade.
 * https://oeis.org/A219999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed by searching decades whose prime pattern is
 *            identical to that of the previous decade
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Static cache of discovered decades */
static seq_t *cache      = NULL;   /* array of decade numbers */
static size_t cache_len  = 0;      /* number of cached entries      */
static size_t cache_cap  = 0;      /* allocated capacity            */
static idx_t  next_d     = 1;      /* next decade to examine       */

/* Ensure cache contains at least 'n' terms. */
static void ensure_cache_up_to(idx_t n)
{
    while (cache_len < n)
    {
        idx_t d = next_d;                 /* current decade        */

        /* Determine prime patterns of previous and current decade. */
        seq_t maskPrev = 0, maskCurr = 0;
        for (idx_t p = 0; p < 10; ++p)
        {
            if (is_prime((d - 1) * 10 + p))
                maskPrev |= (1u << (int)p);
            if (is_prime(d * 10 + p))
                maskCurr |= (1u << (int)p);
        }

        /* If patterns match, store the decade. */
        if (maskPrev == maskCurr)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) abort();
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)d;
        }
        ++next_d;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset<1 */
    ensure_cache_up_to(n);
    return cache[n - 1];
}
