/*
 * A115098  a(0) = 2, a(n) = 3*a(n-1) - 3.
 * https://oeis.org/A115098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2, a(n)=3*a(n-1)-3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    /* limit only for practical usage; tests call n <= 10 000 */
    size_t target = (size_t)n;

    /* initialise cache on first call */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)2;
        cache_len = 1;
    }

    /* grow cache if needed */
    if (target + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < target + 1)
            new_cap = target + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing entries */
    for (size_t i = cache_len; i <= target; ++i) {
        seq_t prev = cache[i - 1];
        seq_t cur = prev * (seq_t)3 - (seq_t)3;
        cache[i] = cur;
    }
    cache_len = target + 1;

    return cache[target];
}
