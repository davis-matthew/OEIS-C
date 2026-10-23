/*
 * A048739  Expansion of 1/((1 - x)*(1 - 2*x - x^2)).
 * https://oeis.org/A048739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 1 with a(0) = 1, a(1) = 3
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t  cache_cap  = 0;   /* allocated size */
    static idx_t   cache_len  = 0;   /* number of computed entries */

    /* initial allocation and seed first two values */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 4;
        while (new_cap <= (size_t)n) {
            new_cap = new_cap * 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n (recurrence for n ≥ 2) */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = (seq_t)2 * cache[i - 1] + cache[i - 2] + (seq_t)1;
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
