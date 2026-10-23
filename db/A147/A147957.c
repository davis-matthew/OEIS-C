/*
 * A147957  a(n) = ((6 + sqrt(2))^n + (6 - sqrt(2))^n)/2.
 * https://oeis.org/A147957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6 + sqrt(2))^n + (6 - sqrt(2))^n)/2 Recurrence: a(0)=1,
 *            a(1)=6, a(n)=12*a(n-1)-34*a(n-2) for n>1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

/* multiply by a small constant; works for both integral and arbitrary‑precision types */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
    return (seq_t)((unsigned __int128)x * m);
#else
    return x * m;
#endif
}

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

#ifdef __cplusplus
    /* C++ implementation using std::vector for dynamic resizing */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
    }
    if (n >= cache.size()) {
        idx_t old = cache.size();
        cache.resize(n + 1);
        for (idx_t i = old; i <= n; ++i) {
            seq_t term = mul_small(cache[i - 1], 12) - mul_small(cache[i - 2], 34);
            cache[i] = term;
        }
    }
    return cache[(size_t)n];
#else
    /* C implementation using malloc/realloc */
    static seq_t *cache   = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;          /* largest index that has been computed */

    if (cache == NULL) {                 /* initialise with two terms */
        cache_cap = 2;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)6;
        cache_len = 1;
    }

    if (n >= cache_cap) {                /* grow capacity sufficiently */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure – terminate */
        cache     = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t term = mul_small(cache[i - 1], 12) - mul_small(cache[i - 2], 34);
        cache[i] = term;
    }
    cache_len = n;
    return cache[(size_t)n];
#endif
}
