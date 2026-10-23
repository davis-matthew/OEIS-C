/*
 * A142276  Primes congruent to 27 mod 43.
 * https://oeis.org/A142276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142276
   Formula: a(n) = nth prime that is congruent to 27 (mod 43)   */

#include "../utils/is_prime.h"
#include <stdlib.h>   /* for realloc */

/* Cache of already found terms */
static seq_t *cache = NULL;            /* array holding a(1), a(2), …      */
static idx_t cache_count = 0;          /* number of terms stored           */
static idx_t cache_cap   = 0;          /* allocated capacity of cache      */
static idx_t last_checked = 0;         /* last candidate tested in the AP  */

seq_t a(idx_t n)
{
    /* Return cached value if already known. */
    if (n <= cache_count)
        return cache[n - 1];

    /* Undefined behaviour for n < 1 (offset is 1).  Here we return 0. */
    if (n < 1)
        return 0;

    /* Start scanning the arithmetic progression 27 + 43k. */
    idx_t cand;
    if (cache_count == 0)
        cand = 27ULL;                   /* first member of the progression */
    else
        cand = last_checked + 43ULL;    /* next candidate after the last checked */

    /* Continue scanning until we have at least n terms. */
    while (cache_count < n)
    {
        if (is_prime(cand))
        {
            /* Grow the cache if necessary. */
            if (cache_count >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_count] = (seq_t)cand;
            cache_count++;
            last_checked = cand;
        }
        cand += 43ULL;   /* move to the next member of the progression */
    }

    return cache[n - 1];
}
