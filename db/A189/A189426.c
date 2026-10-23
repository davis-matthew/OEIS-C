/*
 * A189426  Expansion of (x^2)/(1-2*x-x^2+x^3)^2.
 * https://oeis.org/A189426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=4*a(n-1)-2*a(n-2)-6*a(n-3)+3*a(n-4)+2*a(n-5)-a(n-6) for
 *            n>=6 a(0)=0,a(1)=0,a(2)=1,a(3)=4,a(4)=14,a(5)=42
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* current allocated size */
    static size_t computed_max = 0;  /* highest index whose value is valid */

    /* initial values */
    static const seq_t init[6] = {0, 0, 1, 4, 14, 42};

    size_t idx = (size_t)n;          /* idx_t is uint64_t (unsigned) */

    /* first call: initialise cache with the six initial terms */
    if (cache == NULL) {
        cache_size = 6;                     /* we need indices 0..5 at least */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        for (size_t i = 0; i < cache_size; ++i) {
            cache[i] = init[i];
        }
        computed_max = 5;                   /* last computed index */
    }
    else if (idx >= cache_size) {
        /* enlarge the cache to hold up to idx */
        size_t new_size = idx + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;    /* allocation failed */
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        /* initialise new entries that are still uncomputed */
        for (size_t i = cache_size; i < new_size; ++i) {
            new_cache[i] = (i < 6) ? init[i] : (seq_t)0;
        }
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* if the requested index is already computed, return it */
    if (idx <= computed_max) {
        return cache[idx];
    }

    /* compute missing values up to idx using the recurrence */
    for (size_t i = computed_max + 1; i <= idx; ++i) {
        seq_t A = 4 * cache[i - 1];
        A += 3 * cache[i - 4];
        A += 2 * cache[i - 5];
        seq_t B = 2 * cache[i - 2];
        B += 6 * cache[i - 3];
        B += cache[i - 6];
        cache[i] = A - B;   /* A ≥ B for all n ≥ 6, safe for unsigned types */
    }

    computed_max = idx;            /* update the maximum computed index */
    return cache[idx];
}
