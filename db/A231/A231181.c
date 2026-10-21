/*
 * A231181  Expansion of 1/(1 - x - 4*x^2 + 3*x^3 + 3*x^4 - x^5).
 * https://oeis.org/A231181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 3*a(n-4) + a(n-5) for n >=
 *            5, with a(0)=1, a(1)=1, a(2)=5, a(3)=6, a(4)=20
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* cached values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* initialise the first five entries on the first call */
    if (cache == NULL) {
        cache_cap = 5;
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)1;   /* a(1) */
        cache[2] = (seq_t)5;   /* a(2) */
        cache[3] = (seq_t)6;   /* a(3) */
        cache[4] = (seq_t)20;  /* a(4) */
        cache_len = 5;
    }

    /* resize if required */
    if ((size_t)n >= cache_len) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = NULL;

        if (new_cap > cache_cap) {
#ifdef __cplusplus
            new_cache = new seq_t[new_cap];
#else
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif
            if (new_cache == NULL) return (seq_t)0; /* allocation failure */

            /* copy existing values */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];

            /* free old buffer */
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* compute new terms from cache_len up to n (inclusive) */
        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            seq_t val = cache[k - 1]
                        + 4 * cache[k - 2]
                        - 3 * cache[k - 3]
                        - 3 * cache[k - 4]
                        + cache[k - 5];
            cache[k] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
