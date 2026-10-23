/*
 * A131296  a(n) = ds_5(a(n-1))+ds_5(a(n-2)), a(0)=0, a(1)=1; where
 *          ds_5=digital sum base 5.
 * https://oeis.org/A131296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ds_5(a(n-1)) + ds_5(a(n-2)), a(0)=0, a(1)=1 * ds_5(x) is
 *            the digital sum of x written in base 5. The function * memoises
 *            computed terms in a static dynamic array and returns * any
 *            requested term in O(n) time for the first occurrence of that *
 *            index. Subsequent calls use the stored values
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sum_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sum_base5.h"

/* Dynamic cache shared by all calls */
#if defined(__cplusplus)
static seq_t *cache = nullptr;          /* array of a(i) */
static size_t cache_cap = 0;            /* capacity in elements */
static idx_t  max_computed = 0;         /* highest index computed */
static void grow_cache(idx_t needed)
{
    if ((size_t)(needed + 1) <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if ((size_t)(needed + 1) > new_cap) {
        while ((size_t)(needed + 1) > new_cap)
            new_cap *= 2;
    }
    seq_t *tmp = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            tmp[i] = cache[i];
        delete[] cache;
    }
    cache = tmp;
    cache_cap = new_cap;
}
#else
static seq_t *cache = NULL;            /* array of a(i) */
static size_t cache_cap = 0;            /* capacity in elements */
static idx_t  max_computed = 0;         /* highest index computed */
static void grow_cache(idx_t needed)
{
    if ((size_t)(needed + 1) <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if ((size_t)(needed + 1) > new_cap) {
        while ((size_t)(needed + 1) > new_cap)
            new_cap *= 2;
    }
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();           /* impossible to recover */
    cache = tmp;
    cache_cap = new_cap;
}
#endif

seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (cache == NULL) {
        grow_cache(1);                /* capacity for indices 0 and 1 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n > max_computed) {
        grow_cache(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t v1 = cache[i - 1];
            seq_t v2 = cache[i - 2];
            cache[i] = sum_base5(v1) + sum_base5(v2);
        }
        max_computed = n;
    }
    return cache[n];
}
