/*
 * A091904  Expansion of x/((1+4*x)*(1-8*x)).
 * https://oeis.org/A091904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 32*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow the cache if necessary */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        if (new_size < 2) new_size = 2;      /* at least store a(0) and a(1) */

        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure: best effort */
        cache = tmp;

        /* initialise the first two terms if we just created the cache */
        if (cache_size == 0) {
            cache[0] = (seq_t)0;
            if (new_size > 1) cache[1] = (seq_t)1;
        }

        /* Compute any new terms required */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i <= 1) continue;            /* already set */
            cache[i] = (seq_t)(4 * cache[i-1] + 32 * cache[i-2]);
        }

        cache_size = new_size;
    }

    return cache[n];
}
