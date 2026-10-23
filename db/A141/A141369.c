/*
 * A141369  E.g.f. satisfies A(x) = exp(x*A(-x)).
 * https://oeis.org/A141369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=1; a(n+1) = Σ_{i=0}^{n}
 *            (i+1)*(-1)^i*binomial(n,i)*a(i)*a(n-i) (n≥0)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>      /* for placement new */
#endif

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array with computed terms */
    static idx_t  cache_len = 0;      /* number of terms currently stored */
    static size_t cache_cap = 0;      /* allocated capacity (elements) */

    /* Make sure the base terms a(0)=1 and a(1)=1 are present */
    if (cache_len == 0) {
        /* grow to capacity 2 */
        size_t new_cap = 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0; /* out of memory */
#ifdef __cplusplus
        new(&cache[0]) seq_t(1);
        new(&cache[1]) seq_t(1);
#else
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
#endif
        cache_len = 2;
        cache_cap = new_cap;
    }

    /* If we already know the requested term, return it */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Grow the cache if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < n + 1) new_cap = n + 1;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* out of memory */
        cache = (seq_t*)tmp;
#ifdef __cplusplus
        /* Construct any newly allocated elements */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            new(&cache[i]) seq_t();
        }
#endif
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t cur = cache_len; cur <= n; ++cur) {
        /* For cur = 0 or 1 we already had the values */
        if (cur == 0 || cur == 1) continue;

        idx_t m = cur - 1;          /* m = n in the recurrence */
        seq_t sum = 0;
        seq_t bin  = 1;             /* binomial(m,0) */

        for (idx_t i = 0; i <= m; ++i) {
            /* term = bin * (i+1) * a(i) * a(m-i) */
            seq_t term = bin;
            term *= (seq_t)(i + 1);
            term *= cache[(size_t)i];
            term *= cache[(size_t)(m - i)];

            /* add or subtract according to (-1)^i */
            if (i & 1) {
                sum -= term;
            } else {
                sum += term;
            }

            /* update bin for next i */
            if (i < m) {
                bin *= (seq_t)(m - i);
                bin /= (seq_t)(i + 1);
            }
        }

        cache[(size_t)cur] = sum;
    }

    cache_len = n + 1;  /* all terms up to n are now stored */
    return cache[(size_t)n];
}
