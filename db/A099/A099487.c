/*
 * A099487  Expansion of (1-3x+x^2)/((1+x^2)(1-4x+x^2)).
 * https://oeis.org/A099487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=14;
 *            a(n)=4*a(n-1)-2*a(n-2)+4*a(n-3)-a(n-4) for n≥4, derived from the
 *            generating function (1-3x+x²)/((1+x²)(1-4x+x²))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)14;
        cache_size = 4;
    }

    /* return already computed value if available */
    if (n < cache_size)
        return cache[n];

    /* grow cache so that it can hold indices up to n */
    size_t new_cap = cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = (seq_t)4 * cache[i-1] - (seq_t)2 * cache[i-2]
                    + (seq_t)4 * cache[i-3] - cache[i-4];
        cache[i] = val;
    }
    cache_size = n + 1;

    return cache[n];
}
