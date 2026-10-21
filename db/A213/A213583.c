/*
 * A213583  Principal diagonal of the convolution array A213582.
 * https://oeis.org/A213583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2^(n+2)-3*n-4)/2 (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache of already computed terms (1‑indexed) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of allocated slots (≥ 1) */
static idx_t  cache_max = 0;   /* largest n for which cache[n] is valid */
static seq_t  t_current = 0;   /* t(n) = 2^(n+2) - 3*n - 4 for cache_max */

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* out of range is harmless */

    /* --- 1‑st call: allocate initial cache -------------- */
    if (cache == NULL) {
        cache_cap = 2;                    /* at least indices 0..1 */
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
#endif
        if (!cache) return 0;             /* allocation failure */

        cache[1] = 1;                     /* a(1) */
        cache_max = 1;
        t_current = 1;                    /* t(1) = 2^3 - 3*1 - 4 = 1 */
    }

    /* ---  grows cache if necessary ---------------------- */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 10;   /* generous overshoot */

#ifdef __cplusplus
        seq_t *tmp = new seq_t[new_cap];
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        delete[] cache;
#else
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0; /* allocation failure */
#endif
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ---  cache hit? ------------------------------------- */
    if (n <= cache_max) return cache[n];

    /* ---  compute missing terms up to n -------------------- */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* t(i) = 2*t(i-1) + 3*(i-1) + 1   (recurrence of t) */
        t_current = (t_current << 1) + (seq_t)(3 * (i - 1)) + 1;

        /* a(i) = (i+1)*t(i)/2, division before multiplication
           when (i+1) is odd to keep everything integral              */
        if (((i + 1) & 1ULL) == 0) {                /* (i+1) even */
            cache[i] = ((i + 1) >> 1) * t_current;
        } else {                                   /* (i+1) odd, t_current even */
            cache[i] = (seq_t)(i + 1) * (t_current >> 1);
        }
    }

    cache_max = n;
    return cache[n];
}
