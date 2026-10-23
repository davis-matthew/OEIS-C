/*
 * A016281  Expansion of g.f. 1/((1-2*x)*(1-3*x)*(1-12*x)).
 * https://oeis.org/A016281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/5)*2^(n-1) - 3^(n-1) + (8/5)*12^(n-1) (n≥0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache   = NULL;   /* cached terms of the sequence      */
static size_t cache_cap = 0;    /* allocated capacity of cache       */
static size_t cache_len = 0;    /* number of cached terms (≤ cap)    */

/* expand the cache so that it contains terms up to index n */
static void
ensure_cache(idx_t n)
{
    if (cache_len > (size_t)n)
        return;                           /* already cached */

    /* grow array if necessary */
    if (cache_cap < ((size_t)n + 1))
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* initialise base cases if required */
    if (cache_len == 0)
    {
        cache[0]   = (seq_t)1;
        cache_len  = 1;
    }
    if (cache_len == 1)
    {
        cache[1]   = (seq_t)17;
        cache_len  = 2;
    }
    if (cache_len == 2)
    {
        cache[2]   = (seq_t)223;
        cache_len  = 3;
    }

    /* compute missing terms using the recurrence
       a(n) = 17*a(n-1) - 66*a(n-2) + 72*a(n-3)   for n ≥ 3      */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        seq_t term = ((seq_t)17) * cache[i-1];
        term += ((seq_t)72) * cache[i-3];
        term -= ((seq_t)66) * cache[i-2];
        cache[i] = term;
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
