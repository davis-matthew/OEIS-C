/*
 * A028141  Expansion of 1/((1-4x)(1-6x)(1-9x)(1-12x)).
 * https://oeis.org/A028141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-1) - 342*a(n-2) + 1584*a(n-3) - 2592*a(n-4),
 *            a(0)=1, a(1)=31, a(2)=619, a(3)=10171
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static idx_t cache_cap = 0;      /* capacity of the cache */

/* Expand the cache to hold at least n+1 elements and zero‑initialise
   newly allocated slots. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    /* Zero any newly expanded part of the cache. */
    if (new_cap > cache_cap)
        memset(&tmp[cache_cap], 0, (new_cap - cache_cap) * sizeof(seq_t));

    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the n-th term of A028141. */
seq_t a(idx_t n)
{
    /* Direct constants for the first four terms. */
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)31;
        case 2: return (seq_t)619;
        case 3: return (seq_t)10171;
    }

    ensure_cache(n);

    /* Initialise base terms in the cache if not already done. */
    if (cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)619;
        cache[3] = (seq_t)10171;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue; /* already computed */
        seq_t term = (seq_t)31  * cache[i-1];
        term -= (seq_t)342 * cache[i-2];
        term += (seq_t)1584 * cache[i-3];
        term -= (seq_t)2592 * cache[i-4];
        cache[i] = term;
    }

    return cache[n];
}
