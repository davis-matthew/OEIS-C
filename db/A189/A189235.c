/*
 * A189235  Expansion of
 *          (5-16*x+6*x^2+10*x^3-2*x^4)/(1-4*x+2*x^2+5*x^3-2*x^4-x^5).
 * https://oeis.org/A189235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 5*a(n-3) + 2*a(n-4) + a(n-5);
 *            Initial values: a(0)=5, a(1)=4, a(2)=12, a(3)=25, a(4)=64
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization: cache[i] = a(i)  */
    static seq_t *cache   = NULL;   /* pointer to cache array   */
    static idx_t cache_size = 0;    /* number of valid entries  */
    static idx_t cache_cap  = 0;    /* allocated size           */

    /* ensure capacity for index n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */
        if (cache) {
            for (idx_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* initialise first five terms if not already present */
    if (cache_size < 5) {
        cache[0] = 5;
        cache[1] = 4;
        cache[2] = 12;
        cache[3] = 25;
        cache[4] = 64;
        cache_size = 5;
    }

    /* return if already computed */
    if (n < cache_size)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = 4 * cache[i - 1];
        seq_t t2 = 2 * cache[i - 4];
        seq_t t3 = cache[i - 5];
        seq_t t4 = 2 * cache[i - 2];
        seq_t t5 = 5 * cache[i - 3];

        seq_t temp = t1 + t2 + t3;
        seq_t res  = temp - t4 - t5; /* recurrence: a(n) = temp - t4 - t5 */
        cache[i] = res;
    }
    cache_size = n + 1;
    return cache[n];
}
