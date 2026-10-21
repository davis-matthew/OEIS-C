/*
 * A093985  a(1) = 1, a(2) = 2; a(n+1) = 2n*a(n) - a(n-1). Symmetrically, a(n)
 *          = (a(n-1) + a(n+1))/((n-1) + (n+1)).
 * https://oeis.org/A093985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 2; a(n+1) = 2*n*a(n) - a(n-1) (n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;       /* 0‑based array storing a(1),a(2),… */
static idx_t  cache_max = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;      /* allocated capacity of the array */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        size_t cap = 2;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failed */
        cache[0] = (seq_t)1;                 /* a(1) */
        cache[1] = (seq_t)2;                 /* a(2) */
        cache_max = 2;
        cache_cap = cap;
    }

    if (n == 0)           /* offset starts at 1 – not expected to be used */
        return 0;
    if (n <= cache_max)   /* already computed */
        return cache[n - 1];

    /* compute terms up to n */
    for (idx_t k = cache_max; k < n; ++k) {
        /* grow cache if necessary */
        if ((size_t)(k + 1) > cache_cap) {
            size_t newcap = cache_cap ? cache_cap * 2 : 2;
            while (newcap <= (size_t)(k + 1))
                newcap <<= 1;
            seq_t *newp = (seq_t*)malloc(newcap * sizeof(seq_t));
            if (!newp) return 0;              /* allocation failed */
            memcpy(newp, cache, cache_max * sizeof(seq_t));
            free(cache);
            cache = newp;
            cache_cap = newcap;
        }

        /* recurrence: a(k+1) = 2*k*a(k) - a(k-1) */
        seq_t term = (seq_t)2 * (seq_t)k * cache[k - 1] - cache[k - 2];
        cache[k] = term;
        cache_max++;
    }

    return cache[n - 1];
}
