/*
 * A019623  Expansion of 1/((1-4*x)*(1-7*x)*(1-11*x)).
 * https://oeis.org/A019623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 149*a(n-2) + 308*a(n-3) with a(0)=1, a(1)=22,
 *            a(2)=335
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache for previously computed values */
static seq_t *cache      = NULL;   /* array of computed a(k) */
static idx_t cache_len  = 0;       /* number of valid entries in cache */
static idx_t cache_cap  = 0;       /* allocated capacity of cache */

/* Ensure that the cache contains terms up to index n */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)
        return;

    /* Grow capacity until it can hold n */
    if (cache_cap == 0)
        cache_cap = 8;
    while (cache_cap <= n)
        cache_cap <<= 1;                 /* double until large enough */

    /* Reallocate memory (C++-safe cast) */
    cache = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();                         /* out of memory */

    /* Compute missing terms sequentially */
    for (idx_t i = cache_len; i <= n; ++i) {
        switch (i) {
            case 0u:
                cache[0] = (seq_t)1;
                break;
            case 1u:
                cache[1] = (seq_t)22;
                break;
            case 2u:
                cache[2] = (seq_t)335;
                break;
            default:
                /* Use a stable order to avoid underflow on unsigned types */
                seq_t term = (seq_t)22 * cache[i-1]
                           + (seq_t)308 * cache[i-3];
                term -= (seq_t)149 * cache[i-2];
                cache[i] = term;
                break;
        }
    }
    cache_len = n + 1;
}

/* Return the n-th term of the sequence (0‑based) */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
