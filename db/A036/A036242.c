/*
 * A036242  Numerator of fraction equal to the continued fraction
 *          [0,2,4,...2n].
 * https://oeis.org/A036242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = (2*(n-1)+2)*a(n-1)+a(n-2) for n ≥ 3, with a(0) = 1, a(1)
 *            = 1, a(2) = 4. (Recurrence from OEIS A036242)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* cache for memoised values */
static seq_t *cache       = NULL; /* buffer of computed a(n) */
static size_t cache_cap   = 0;    /* allocated capacity */
static idx_t  cache_last  = 0;    /* index of last valid entry in cache */

/* ensure cache has room for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* initial allocation up to index 3 (we will store a(0),a(1),a(2)) */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache_last = 2;
    }

    if (n <= cache_last) return;

    /* grow capacity if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values */
    for (idx_t k = cache_last + 1; k <= n; ++k) {
        idx_t mul = 2 * (k - 1) + 2;            /* multiplier (2k) */
        cache[k] = (seq_t)mul * cache[k - 1] + cache[k - 2];
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1; /* a(0) is defined for recurrence */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    ensure_cache(n);
    return cache[n];
}
