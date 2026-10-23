/*
 * A131295  a(n)=ds_4(a(n-1))+ds_4(a(n-2)), a(0)=0, a(1)=1; where ds_4=digital
 *          sum base 4.
 * https://oeis.org/A131295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n)=ds_4(a(n-1))+ds_4(a(n-2)), a(0)=0, a(1)=1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ds_4 = digital sum in base 4 */

#include <stdlib.h>

static inline seq_t digit_sum_base4(seq_t v)
{
    seq_t s = 0;
    while (v) {
        s += v % 4;
        v /= 4;
    }
    return s;
}

#ifdef __cplusplus
#  define ALLOC(type, count) reinterpret_cast<type*>(malloc(sizeof(type)*(count)))
#  define REALLOC(ptr, type, count) reinterpret_cast<type*>(realloc(ptr, sizeof(type)*(count)))
#else
#  define ALLOC(type, count) ((type*)malloc(sizeof(type)*(count)))
#  define REALLOC(ptr, type, count) ((type*)realloc(ptr, sizeof(type)*(count)))
#endif

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* initialise cache with first two values */
    if (!cache) {
        cache_cap = 4;
        cache = ALLOC(seq_t, cache_cap);
        cache[0] = 0;
        cache[1] = 1;
        cache_len = 2;
    }
    /* return if already computed */
    if (n < cache_len)
        return cache[n];

    /* grow cache as needed */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = REALLOC(cache, seq_t, new_cap);
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = digit_sum_base4(cache[i - 1]) + digit_sum_base4(cache[i - 2]);
    }
    cache_len = n + 1;

    return cache[n];
}
