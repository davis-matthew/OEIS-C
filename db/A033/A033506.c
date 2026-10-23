/*
 * A033506  Number of matchings in graph P_{3} X P_{n}.
 * https://oeis.org/A033506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=22, a(3)=131, a(4)=823, a(5)=5096,
 *            a(n)=4*a(n-1)+14*a(n-2)-10*a(n-4)+a(n-6) for n≥6
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static const seq_t BASE[6] = {
    (seq_t)1,  (seq_t)3,  (seq_t)22,
    (seq_t)131, (seq_t)823, (seq_t)5096
};

static seq_t *cache = NULL;     /* dynamically grown cache of terms */
static size_t cache_size = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        /* start with a reasonable capacity (at least 16 or n+1) */
        cache_cap = 16;
        if (cache_cap < (size_t)n + 1)
            cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */

        /* store the first 6 known terms */
        for (size_t i = 0; i < 6; ++i)
            cache[i] = BASE[i];
        cache_size = 6;
    }

    /* ensure capacity for n */
    while (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= cache_cap)  /* overflow protection */
            new_cap = cache_cap + 1;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 6) {
            /* already initialised */
            cache[i] = BASE[i];
        } else {
            seq_t t1 = mul_small(cache[i - 1], 4);
            seq_t t2 = mul_small(cache[i - 2], 14);
            seq_t t3 = mul_small(cache[i - 4], 10);
            seq_t sum = t1;
            sum += t2;
            sum += cache[i - 6];
            sum -= t3;
            cache[i] = sum;
        }
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
