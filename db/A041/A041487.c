/*
 * A041487  Denominators of continued fraction convergents to sqrt(260).
 * https://oeis.org/A041487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A041487: Denominators of continued fraction convergents to sqrt(260).
 *
 * Recurrence: a(n) = 258*a(n-2) - a(n-4)   (n > 3)
 * Initial values:
 *   a(0) = 1
 *   a(1) = 8
 *   a(2) = 257
 *   a(3) = 2064
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cnt  = 0;   /* number of values currently stored, 0..n */
    static size_t cache_cap  = 0;   /* current allocation capacity */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 1;
        cache[1] = 8;
        cache[2] = 257;
        cache[3] = 2064;
        cache_cnt = 4;
    }

    /* if the requested term is already computed, return it */
    if (n < cache_cnt) return cache[n];

    /* ensure capacity for indices up to n */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < n + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = cache_cnt; i <= n; ++i) {
        /* recurrence only applies for i >= 4; i < 4 cannot occur here */
        seq_t tmp = 258 * cache[i - 2];
        cache[i] = tmp - cache[i - 4];
    }

    cache_cnt = n + 1;   /* now we have values 0..n stored */
    return cache[n];
}
