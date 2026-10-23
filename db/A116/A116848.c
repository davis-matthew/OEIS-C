/*
 * A116848  Number of permutations of length n which avoid the patterns 231,
 *          51234.
 * https://oeis.org/A116848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 14*a(n-2) + 17*a(n-3) - 10*a(n-4) + 4*a(n-5)
 *            (n>5) Initial values: a(1)=1, a(2)=2, a(3)=5, a(4)=14, a(5)=41
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] unused, cache[i] = a(i) */
static idx_t cache_cap = 0;          /* allocated size (including index 0) */
static idx_t cache_len = 0;          /* maximum n that has been computed */

seq_t a(idx_t n)
{
    /* return precomputed constants for n <= 5 */
    if (n <= 5) {
        switch ((unsigned)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)5;
            case 4: return (seq_t)14;
            case 5: return (seq_t)41;
        }
    }

    /* ensure we have a cache that can hold a(n) */
    if (cache_len < 5) {
        /* initial allocation to at least 6 elements */
        idx_t need = (n > 5) ? (n + 1) : 6;
        if (need > cache_cap) {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)need);
            cache_cap = need;
        }
        /* store base values in cache if not already */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)5;
        cache[4] = (seq_t)14;
        cache[5] = (seq_t)41;
        cache_len = 5;
    }

    if (n > cache_len) {
        /* extend cache if necessary */
        if (n + 1 > cache_cap) {
            idx_t new_cap = n + 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)new_cap);
            cache_cap = new_cap;
        }
        /* compute missing terms up to n */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            cache[i] = 6 * cache[i - 1] 
                       - 14 * cache[i - 2] 
                       + 17 * cache[i - 3] 
                       - 10 * cache[i - 4] 
                       + 4  * cache[i - 5];
        }
        cache_len = n;
    }

    return cache[n];
}
