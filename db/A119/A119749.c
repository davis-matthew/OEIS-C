/*
 * A119749  Number of compositions of n into odd blocks with one element in
 *          each block distinguished.
 * https://oeis.org/A119749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=4, a(4)=7; for n>4 a(n) = a(n-1) + 2*a(n-2)
 *            + a(n-3) - a(n-4) (linear recurrence)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* array of a(k), indices 0..max_n */
static idx_t  cache_cap = 0;         /* allocated size (elements)      */
static idx_t  cache_max = 0;         /* largest n currently stored     */

static void ensure_cache(idx_t n)
{
    /* Allocate/expand the cache if necessary.  All new entries
       are zero-initialised; base values are written later. */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) return;          /* OOM – silently fail; caller will obtain 0 */
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = 0;
        cache_max = 0;
    } else if (n >= cache_cap - 1) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return;            /* OOM – silently fail */
        for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Initialise the first four values if they are not yet stored. */
    if (cache_max < 1 && cache_cap > 1) cache[1] = (seq_t)1;
    if (cache_max < 2 && cache_cap > 2) cache[2] = (seq_t)1;
    if (cache_max < 3 && cache_cap > 3) cache[3] = (seq_t)4;
    if (cache_max < 4 && cache_cap > 4) cache[4] = (seq_t)7;

    /* Compute missing terms up to n.  For n<=4 the loop just skips. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 4) continue;               /* base values already set */
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t two_t2 = t2 + t2;             /* 2*a(n-2) */
        cache[i] = t1 + two_t2 + t3 - t4;   /* recurrence */
    }
    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;            /* undefined for n<1 */
    ensure_cache(n);
    return cache[n];
}
