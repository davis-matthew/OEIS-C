/*
 * A077832  Expansion of 1/(1-2*x-3*x^2-3*x^3).
 * https://oeis.org/A077832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=7, a(n)=2*a(n-1)+3*a(n-2)+3*a(n-3) for n>=3
 *            (coefficients from 1/(1-2*x-3*x^2-3*x^3) )
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms cached */
static idx_t cache_cap = 0;   /* allocated capacity */

static void grow_cache(idx_t upto)
{
    if (upto < cache_len)
        return;
    if (cache_cap < upto + 1) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= upto)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* ensure the cache can hold up to n */
    grow_cache(n);

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)2;
        } else if (i == 2) {
            cache[2] = (seq_t)2 * cache[1] + (seq_t)3 * cache[0];
        } else {
            cache[i] = (seq_t)2 * cache[i-1] + (seq_t)3 * cache[i-2] + (seq_t)3 * cache[i-3];
        }
    }
    cache_len = n + 1;
    return cache[n];
}
