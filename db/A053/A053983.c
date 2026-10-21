/*
 * A053983  a(n) = (2*n-1)*a(n-1) - a(n-2), a(0)=a(1)=1.
 * https://oeis.org/A053983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A053983
   Formula: a(n) = (2*n-1)*a(n-1) - a(n-2),  a(0)=1, a(1)=1 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Allocate and/or enlarge the cache if necessary */
    if (cache == NULL) {
        cache_cap = 8;                     /* initial capacity */
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }
    while (cache_cap <= n) {
        cache_cap <<= 1;                   /* double until enough */
        cache = (seq_t *)realloc(cache,
                                 sizeof(seq_t) * cache_cap);
    }

    /* Compute terms up to n and store them */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            cache[i] = 1;
        } else {
            seq_t prod = (seq_t)(2 * i - 1) * cache[i - 1];
            cache[i]  = prod - cache[i - 2];
        }
    }
    cache_len = n + 1;                     /* update cached range */

    return cache[n];
}
