/*
 * A163988  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^6 = I.
 * https://oeis.org/A163988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=22, a(2)=462, a(3)=9702, a(4)=203742, a(5)=4278582,
 *            a(6)=89849991
 *            a(n)=20*a(n-1)+20*a(n-2)+20*a(n-3)+20*a(n-4)+20*a(n-5)-210*a(n-6)
 *            for all n≥7
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    size_t target = (size_t)n;
    size_t required = target + 1;
    if (required < 7) required = 7;           /* need the first 7 terms */

    /* Allocate or grow the cache. */
    cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
    if (!cache) return 0; /* graceful failure if out of memory */

    /* Initialise the base values once. */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)462;
        cache[3] = (seq_t)9702;
        cache[4] = (seq_t)203742;
        cache[5] = (seq_t)4278582;
        cache[6] = (seq_t)89849991;
        cache_size = 7;
    }

    /* Compute any missing terms up to 'required'. */
    for (size_t i = cache_size; i < required; ++i) {
        seq_t sum = 20 * cache[i-1] + 20 * cache[i-2]
                  + 20 * cache[i-3] + 20 * cache[i-4]
                  + 20 * cache[i-5];
        seq_t sub = 210 * cache[i-6];
        cache[i] = sum - sub;               /* guaranteed non‑negative */
    }

    cache_size = required;
    return cache[target];
}
