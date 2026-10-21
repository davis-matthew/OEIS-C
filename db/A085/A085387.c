/*
 * A085387  E.g.f exp(x)*cosh(x+x^2/2).
 * https://oeis.org/A085387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + (2*n-3)*a(n-3) + (n-3)^2*a(n-4) -
 *            (n-3)*(n-4)*a(n-5), a(0)=1,a(1)=1,a(2)=2,a(3)=7,a(4)=23
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* make sure the cache has room for a[0..n] */
    if ((size_t)n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first five terms once */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)7;
        cache[4] = (seq_t)23;
        cache_len = 5;
    }

    /* already computed? */
    if (n < cache_len) return cache[n];

    /* compute terms up to n using the recurrence */
    for (size_t k = cache_len; k <= n; ++k)
    {
        seq_t t1 = cache[k-1] * (seq_t)3;
        seq_t t2 = cache[k-2] * (seq_t)2;
        seq_t t3 = cache[k-3] * (((seq_t)k * (seq_t)2) - (seq_t)3);
        seq_t t4 = cache[k-4] * (((seq_t)(k-3) * (seq_t)(k-3)));
        seq_t t5 = cache[k-5] * (((seq_t)(k-3) * (seq_t)(k-4)));

        seq_t val = t1 - t2 + t3 + t4 - t5;
        cache[k] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
