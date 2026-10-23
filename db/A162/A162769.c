/*
 * A162769  a(n) = ((1+sqrt(5))*(4+sqrt(5))^n + (1-sqrt(5))*(4-sqrt(5))^n)/2.
 * https://oeis.org/A162769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 11*a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= needed)
        new_cap *= 2;
#ifdef __cplusplus
    seq_t *tmp = reinterpret_cast<seq_t *>(realloc(cache, new_cap * sizeof(seq_t)));
#else
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    if (cache_len <= n) {
        /* prepare cache up to index n */
        ensure_cap((size_t)n + 1);
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)9;
            cache_len = 2;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)8 * cache[i - 1] - (seq_t)11 * cache[i - 2];
        }
        cache_len = n + 1;
    }
    return cache[n];
}
