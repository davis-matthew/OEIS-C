/*
 * A086931  a(0) = 1, a(n) = lpf(n)*a(n-lpf(n)), where lpf=A020639 (least
 *          prime factor).
 * https://oeis.org/A086931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lpf(n) * a(n - lpf(n)), a(0)=1, a(1)=1 (lpf = least prime
 *            factor, defined in "../utils/lpf.h")
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                /* malloc, realloc, abort */
#include "../utils/lpf.h"          /* idx_t lpf(idx_t) */

static seq_t *cache   = NULL;      /* memoization cache, 0…n */
static idx_t  cache_sz = 0;        /* number of usable entries in cache */

/* compute a(n) for any n >= 0, with memoization */
seq_t a(idx_t n)
{
    /* initialisation: store a(0)=1 and a(1)=1 */
    if (cache_sz == 0) {
        cache_sz = 2;                       /* indices 0 and 1 */
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) abort();                /* out of memory */
        cache[0] = 1;                       /* a(0) */
        cache[1] = 1;                       /* a(1) */
    }

    /* extend cache if needed */
    if (n >= cache_sz) {
        size_t newsize = (size_t)(n + 1);    /* inclusive up to n */
        seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newcache) abort();              /* out of memory */
        cache = newcache;
        idx_t oldsize = cache_sz;
        cache_sz = (idx_t)newsize;

        /* compute missing terms from oldsize to n */
        for (idx_t i = oldsize; i <= n; ++i) {
            if (i < 2) {                    /* a(0)=a(1)=1 already set */
                cache[i] = 1;
                continue;
            }
            idx_t l = lpf(i);                /* least prime factor of i */
            cache[i] = cache[i - l] * (seq_t)l;
        }
    }

    return cache[n];
}
