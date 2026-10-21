/*
 * A212415  Number of (w,x,y,z) with all terms in {1,...,n} and w<x>=y<=z.
 * https://oeis.org/A212415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 378, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5),
   with a(0)=0, a(1)=0, a(2)=3, a(3)=17, a(4)=55. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0..4 */
    static const seq_t base[5] = {0, 0, 3, 17, 55};

    /* Cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Handle the trivial cases directly */
    if (n < 5)
        return base[(idx_t)n];

    /* Ensure the cache is large enough */
    if (cache_size < n + 1) {
        /* Allocate or grow the cache */
        if (cache == NULL) {
            /* First allocation: initialise with base values */
            cache = (seq_t *)malloc((sizeof(seq_t) * (n + 1)));
            if (!cache) return 0;   /* Allocation failure -> zero */
            for (idx_t i = 0; i < 5; ++i)
                cache[i] = base[i];
            cache_size = 5;
        } else {
            /* Grow the existing cache */
            seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
            if (!new_cache) return 0;   /* Allocation failure -> zero */
            cache = new_cache;
            /* Compute new entries */
        }

        /* Fill in new entries up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t1 = 5 * cache[i - 1];
            seq_t t2 = 10 * cache[i - 2];
            seq_t t3 = 10 * cache[i - 3];
            seq_t t4 = 5 * cache[i - 4];
            seq_t val = t1 - t2 + t3 - t4 + cache[i - 5];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
