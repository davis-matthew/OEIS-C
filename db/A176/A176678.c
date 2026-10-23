/*
 * A176678  a(n+1) = Sum_{p=0..n} a(p)*a(n-p)+k+l for n>=1, with here a(0)=1,
 *          a(1)=2, k=0 and l=-1.
 * https://oeis.org/A176678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=2, a(n+1)=∑_{p=0}^{n} a(p)*a(n-p)-1 (n≥1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for memoised values */
static seq_t *cache = NULL;
static size_t cap   = 0;
static idx_t last   = 0;    /* highest index already computed */

/* ensure the cache is large enough for index n */
static void _ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cap < needed) {
        size_t new_cap = needed;
        seq_t *new_cache;
#ifdef __cplusplus
        new_cache = new seq_t[new_cap];          /* works for classes too */
#else
        new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
        if (cache) {
#ifdef __cplusplus
            for (size_t i = 0; i < cap; ++i)
                new_cache[i] = cache[i];
            delete [] cache;
#else
            memcpy(new_cache, cache, cap * sizeof(seq_t));
            free(cache);
#endif
        }
        cache = new_cache;
        cap   = new_cap;
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        _ensure_cache(0);
        cache[0] = 1;
        last = 0;
    }

    if (n == 0) return cache[0];

    _ensure_cache(n);

    /* initialise a(1) if not already done */
    if (last < 1) {
        cache[1] = 2;
        last = 1;
    }

    /* compute missing values up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t p = 0; p < i; ++p)
                sum += cache[(size_t)p] * cache[(size_t)(i - 1 - p)];
            cache[(size_t)i] = sum - 1;
        }
        last = n;
    }

    return cache[(size_t)n];
}
