/*
 * A063199  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          11 ).
 * https://oeis.org/A063199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-5) for n ≥ 7 (with initial values
 *            a(1)=1, a(2)=2, a(3)=4, a(4)=6, a(5)=8, a(6)=8)
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to avoid recomputation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest n cached (inclusive) */

    if (n == 0)                /* undefined for n=0, return 0 as safe value */
        return 0;

    /* extend cache if needed */
    if (n > cache_size) {
        /* allocate space for indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL)  /* allocation failed: return 0 */
            return 0;
        cache = new_cache;

        /* compute new entries */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1)            cache[i] = (seq_t)1;
            else if (i == 2)       cache[i] = (seq_t)2;
            else if (i == 3)       cache[i] = (seq_t)4;
            else if (i == 4)       cache[i] = (seq_t)6;
            else if (i == 5)       cache[i] = (seq_t)8;
            else if (i == 6)       cache[i] = (seq_t)8;
            else                    /* recurrence for i ≥ 7   */
                cache[i] = cache[i-2] + cache[i-3] - cache[i-5];
        }
        cache_size = n;
    }

    return cache[n];
}
