/*
 * A116553  a(2*n+1) = 5*a(n), a(2*n+2) = 6*a(n) + a(n-1).
 * https://oeis.org/A116553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n+1) = 5*a(n), a(2*n+2) = 6*a(n) + a(n-1)
 * Range:     u8 n <= 13, u16 n <= 123, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* choose a size that covers the usual test range (10 000) */
#define MAX_N 20000

seq_t a(idx_t n)
{
    /* cache keeps all computed values */
    static seq_t cache[MAX_N + 1];
    static idx_t cached_upto = 0;   /* next index that has not yet been computed */

    /* initialise base cases on first call */
    if (cached_upto == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)6;
        cached_upto = 3;   /* indices 0,1,2 are ready */
    }

    /* grow the cache if the requested index is beyond the current limit */
    if (n >= cached_upto) {
        if (n > MAX_N) {
            /* if n is larger than the static buffer, grow it dynamically.
               Use C++ new[] or C malloc depending on the compiler. */
            seq_t *new_cache;
            size_t new_cap = (size_t)n + 1;
#ifdef __cplusplus
            new_cache = new seq_t[new_cap];
            /* copy existing values */
            for (idx_t i = 0; i < cached_upto; ++i)
                new_cache[i] = cache[i];
            /* delete old static array is impossible – we keep the static
               array for the first MAX_N entries and only use dynamic beyond that. */
#else
            new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#endif
            /* fallback to static buffer if allocation fails */
            if (!new_cache) new_cache = cache;
            /* compute beyond the old limit */
            for (idx_t i = cached_upto; i <= n; ++i) {
                if ((i & 1u) == 1u) {                      /* odd */
                    idx_t k = (i - 1u) >> 1;
                    new_cache[i] = (seq_t)5 * new_cache[k];
                } else {                                    /* even */
                    idx_t k = (i - 2u) >> 1;
                    seq_t val = (seq_t)6 * new_cache[k];
                    if (k >= 1u) val += new_cache[k - 1u];
                    new_cache[i] = val;
                }
            }
            cached_upto = n + 1;
            /* use new_cache for the result */
            return new_cache[n];
        }
        /* compute within the static buffer */
        for (idx_t i = cached_upto; i <= n; ++i) {
            if ((i & 1u) == 1u) {                      /* odd */
                idx_t k = (i - 1u) >> 1;
                cache[i] = (seq_t)5 * cache[k];
            } else {                                    /* even */
                idx_t k = (i - 2u) >> 1;
                seq_t val = (seq_t)6 * cache[k];
                if (k >= 1u) val += cache[k - 1u];
                cache[i] = val;
            }
        }
        cached_upto = n + 1;
    }

    return cache[n];
}
