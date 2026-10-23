/*
 * A015195  Sum of Gaussian binomial coefficients for q=9.
 * https://oeis.org/A015195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+(9^(n-1)-1)*a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* static memoisation state */
    static seq_t *cache = NULL;   /* array a(0) … a(lastComputed) */
    static idx_t  lastComputed = 0; /* highest index already computed */
    static size_t cap = 0;          /* allocated size of cache */
    static seq_t pow9 = 1;          /* 9^lastComputed */

    /* Ensure cache size is at least n+1 */
    if (cap < (size_t)(n + 1)) {
        size_t newCap = (size_t)(n + 1);
        seq_t *newCache;
#ifdef __cplusplus
        newCache = new seq_t[newCap];
        if (cache) {
            for (size_t i = 0; i < cap; ++i)
                newCache[i] = cache[i];
            delete[] cache;
        }
#else
        newCache = (seq_t*)malloc(newCap * sizeof(seq_t));
        if (!newCache) abort();
        if (cache) {
            memcpy(newCache, cache, cap * sizeof(seq_t));
            free(cache);
        }
#endif
        cache = newCache;
        cap = newCap;
    }

    /* Initialise base terms if this is the first non‑trivial call. */
    if (lastComputed < 1) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        lastComputed = 1;
        pow9 = (seq_t)9;   /* 9^1 */
    }

    /* Compute missing terms up to n. */
    for (idx_t i = lastComputed + 1; i <= n; ++i) {
        cache[i] = (seq_t)2 * cache[i - 1] + (pow9 - (seq_t)1) * cache[i - 2];
        pow9 *= (seq_t)9;
    }
    lastComputed = n;
    return cache[n];
}
