/*
 * A177728  Expansion of (1 + 14*x) / ((1 - x)*(1 - 2*x)*(1 - 4*x)*(1 -
 *          8*x)*(1 - 16*x)).
 * https://oeis.org/A177728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/21)*((-1 + 2^(1+n))^2*(1-3*2^(2+n) + 2^(5+2*n)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n ≥ 5:
   a(n) = 31*a(n-1) - 310*a(n-2) + 1240*a(n-3) - 1984*a(n-4) + 1024*a(n-5) */
#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values          */
static idx_t cache_len = 0;          /* current length of cache         */
static idx_t cache_cap = 0;          /* allocated capacity of cache     */

/* Ensure the cache can store at least 'needed' terms (indexable 0..needed-1). */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= needed)
        new_cap *= 2U;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache with first 5 terms (a(0) .. a(4)). */
    if (cache_len == 0) {
        ensure_cache_capacity(5);
        cache[0] = 1;
        cache[1] = 45;
        cache[2] = 1085;
        cache[3] = 20925;
        cache[4] = 366141;
        cache_len = 5;
    }

    /* Return already computed value if available. */
    if (n < cache_len)
        return cache[n];

    /* Compute missing terms up to n using the linear recurrence. */
    for (idx_t k = cache_len; k <= n; ++k) {
        ensure_cache_capacity(k + 1);
        seq_t term;

#ifdef __USE_BUILTIN_POPCOUNT__   /* pragma for style; harmless if not defined */
#endif
        /* Compute a(k) = 31*a(k-1) - 310*a(k-2) + 1240*a(k-3)
         *          - 1984*a(k-4) + 1024*a(k-5) */
        seq_t acc = cache[k-1] * 31;
        acc -= cache[k-2] * 310;
        acc += cache[k-3] * 1240;
        acc -= cache[k-4] * 1984;
        acc += cache[k-5] * 1024;
        term = acc;

        cache[k] = term;
    }

    cache_len = n + 1;
    return cache[n];
}
