/*
 * A095176  E.g.f.: exp(9x)/(1-9x)^(1/9).
 * https://oeis.org/A095176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) + (-9*n-1)*a(n-1) + 81*(n-1)*a(n-2) = 0, with a(0)=1,
 *            a(1)=10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache     = NULL;   /* array holding computed terms   */
static idx_t cache_size = 0;      /* number of terms currently stored (next free index) */

/* Extend the cache so that it contains terms up to nt (inclusive). */
static void grow_cache(idx_t nt)
{
    if (nt + 1 <= cache_size)      /* already have all needed terms */
        return;

    /* Reallocate the cache to hold nt+1 entries. */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)(nt + 1) * sizeof(seq_t));
    if (!tmp) {                    /* allocation failed – terminate */
        exit(1);
    }
    cache = tmp;

    /* Compute missing terms from current cache_size up to nt. */
    for (idx_t i = cache_size; i <= nt; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 10;
        } else {
            seq_t t1 = ((seq_t)i * 9 + 1) * cache[i - 1];
            seq_t t2 = ((seq_t)(i - 1) * 81) * cache[i - 2];
            cache[i] = t1 - t2;
        }
    }

    cache_size = nt + 1;
}

seq_t a(idx_t n)
{
    grow_cache(n);
    return cache[n];
}
