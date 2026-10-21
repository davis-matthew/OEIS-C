/*
 * A200864  Expansion of 1/((1+x)*(1-3*x)*(1-5*x)).
 * https://oeis.org/A200864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (50*5^n-27*3^n+(-1)^n)/24 Recurrence used: a(0)=1,
 *            a(1)=7, a(i)=2*a(i-1)+3*a(i-2)+5^i for i>1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Computes a(n) for OEIS A200864 */
seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> cache;      /* memoised terms */
    static seq_t pow5(1);                 /* 5^i (i equals current i after update) */
    const size_t cache_len = cache.size();

    if (n >= cache_len) {
        /* extend cache to at least n+1 */
        cache.resize(n + 1);

        /* compute missing terms */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[0] = seq_t(1);           /* a(0) = 1 */
            } else if (i == 1) {
                cache[1] = seq_t(7);           /* a(1) = 7 */
            } else {
                /* recurrence uses current pow5 = 5^i */
                cache[i] = seq_t(2) * cache[i - 1]
                         + seq_t(3) * cache[i - 2]
                         + pow5;
            }
            /* advance pow5 to 5^(i+1) for next iteration */
            pow5 *= seq_t(5);
        }
    }
    return cache[(size_t)n];
#else
    static seq_t *cache = NULL; /* memoised terms (C mode) */
    static idx_t cache_len = 0; /* current number of cached terms */
    static seq_t pow5 = 1;      /* 5^i, i equals current i after update */

    if (n < cache_len) {
        return cache[n];
    }

    /* extend storage */
    idx_t new_len = n + 1;
    cache = realloc(cache, new_len * sizeof(*cache));
    if (!cache) {
        /* memory allocation failed – return 0 as a fallback */
        return (seq_t)0;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;  /* a(0) = 1 */
        } else if (i == 1) {
            cache[1] = (seq_t)7;  /* a(1) = 7 */
        } else {
            /* recurrence uses current pow5 = 5^i */
            cache[i] = (seq_t)2 * cache[i - 1] + (seq_t)3 * cache[i - 2] + pow5;
        }
        /* advance pow5 to 5^(i+1) for the next iteration */
        pow5 *= (seq_t)5;
    }

    cache_len = new_len;
    return cache[n];
#endif
}
