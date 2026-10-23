/*
 * A156087  One ninth of the sum of the squares of the first n Fibonacci
 *          numbers with index divisible by 4.
 * https://oeis.org/A156087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=50 Recurrence: a(n) = 48*a(n-1) - 48*a(n-2)
 *            + a(n-3) + 2 (n>=3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Static cache to store computed values for all calls. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* allocated array length                */
    static idx_t cache_max = (idx_t)-1; /* last index for which values are set */

    /* Allocate and initialise the cache on the first call. */
    if (cache == NULL) {
        size_t init_len = (n + 1 > 3) ? (n + 1) : 3;            /* need at least 3 slots */
        cache = (seq_t *)malloc(init_len * sizeof(seq_t));
        if (!cache) return 0;                                 /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)50;
        cache_len = init_len;
        cache_max = 2;
    }

    /* Grow the cache if necessary. */
    if (cache_len <= n) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;                                 /* reallocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute any missing terms using the recurrence. */
    if (n > cache_max) {
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t term = (seq_t)48 * cache[i - 1]
                       - (seq_t)48 * cache[i - 2]
                       + cache[i - 3]
                       + (seq_t)2;
            cache[i] = term;
        }
        cache_max = n;
    }

    return cache[n];
}
