/*
 * A041251  Denominators of continued fraction convergents to sqrt(137).
 * https://oeis.org/A041251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3488*a(n-9) + a(n-18) for n ≥ 18
   initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=7, a(4)=10,
   a(5)=17, a(6)=44, a(7)=105, a(8)=149, a(9)=3383,
   a(10)=3532, a(11)=10447, a(12)=24426, a(13)=34873,
   a(14)=59299, a(15)=153471, a(16)=366241, a(17)=519712 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache           = NULL;
    static idx_t  cache_len       = 0;    /* current capacity of cache array */
    static idx_t  cache_max       = 17;   /* highest index computed and stored */

    /* small table of the first 18 terms, used for direct look‑up and
       for initializing the cache */
    static const seq_t init[18] = {
        (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)7,  (seq_t)10,
        (seq_t)17, (seq_t)44, (seq_t)105, (seq_t)149,
        (seq_t)3383, (seq_t)3532, (seq_t)10447, (seq_t)24426,
        (seq_t)34873, (seq_t)59299, (seq_t)153471, (seq_t)366241,
        (seq_t)519712
    };

    /* trivial case: n < 18 */
    if (n < 18) {
        return init[n];
    }

    /* initialize cache on first use */
    if (cache == NULL) {
        cache_len = 18;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 18; ++i) {
            cache[i] = init[i];
        }
        /* cache_max already 17 */
    }

    /* if value already computed, just return it */
    if (n <= cache_max) {
        return cache[n];
    }

    /* grow the cache if required */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failed – fall back to reallocating a fresh array
               and recomputing from scratch so that the function keeps
               working.  This situation is unlikely for realistic values
               of n. */
            cache_len = 18;
            cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
            for (idx_t i = 0; i < 18; ++i) cache[i] = init[i];
            cache_max = 17;
        } else {
            cache = tmp;
            cache_len = new_len;
        }
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)3488 * cache[i - 9] + cache[i - 18];
    }
    cache_max = n;

    return cache[n];
}
