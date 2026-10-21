/*
 * A213967  a(n)=n for n<=3; thereafter a(n)=a(n-1)+a(n-2)+a(n-3)+1.
 * https://oeis.org/A213967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + 1 for n >= 4; Initial values:
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization 					      */
    static seq_t  *cache  = NULL;
    static size_t cache_cap = 0;
    static idx_t  max_n  = 0;

    /* initialize cache with first four terms       */
    if (!cache) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure (unlikely) */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        max_n  = 3;                 /* indices 0..3 are known          */
    }

    /* serve cached value when already computed     */
    if (n <= max_n) return cache[n];

    /* extend memory if necessary                    */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1)) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure (unlikely) */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n                 */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        if (i <= 3)
            cache[i] = (seq_t)i;
        else
            cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + (seq_t)1;
    }

    max_n = n;                      /* update maximum computed index   */
    return cache[n];
}
