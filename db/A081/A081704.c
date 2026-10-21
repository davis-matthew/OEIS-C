/*
 * A081704  Let f(0)=1, f(1)=t, f(n+1) = (f(n)^2+t^n)/f(n-1). f(t) is a
 *          polynomial with integer coefficients. Then a(n) = f(n) when t=3.
 * https://oeis.org/A081704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(n+1)=5*a(n)-3*a(n-1) (t=3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    if (cache_len == 0)
    {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* fallback if allocation fails */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    if (n + 1 > cache_len)
    {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* fallback if reallocation fails */
        cache = new_cache;
        for (idx_t i = cache_len; i <= n; ++i)
        {
            cache[i] = (seq_t)5 * cache[i-1] - (seq_t)3 * cache[i-2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
