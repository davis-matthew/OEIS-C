/*
 * A067011  a(2n) and a(2n+1) are side lengths of a Beentjes sequence of
 *          perfect squared rectangles, starting with a 33 X 32 rectangle.
 * https://oeis.org/A067011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=33, a(1)=32, a(2)=683, a(3)=781, * a(n)=22*a(n-2)+9*a(n-4)
 *            for n>3
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_cached = 0;

    /* initialise cache with the first four terms */
    if (!cache) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();
        cache[0] = 33;
        cache[1] = 32;
        cache[2] = 683;
        cache[3] = 781;
        max_cached = 3;
    }

    /* grow the cache if we need to compute further terms */
    if (n > max_cached) {
        /* ensure space up to n */
        if ((idx_t)cache_size <= n) {
            size_t new_size = cache_size;
            while (new_size <= (size_t)n) {
                new_size *= 2;
                if (new_size == 0ul) new_size = n + 1;   /* overflow guard */
            }
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
            if (!tmp) abort();
            cache = tmp;
            cache_size = new_size;
        }

        /* compute missing terms using the recurrence */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* a(i) = 22*a(i-2) + 9*a(i-4) */
            seq_t term1 = mul_small(cache[i - 2], 22);
            seq_t term2 = mul_small(cache[i - 4], 9);
            cache[i] = term1 + term2;
        }
        max_cached = n;
    }

    return cache[n];
}
