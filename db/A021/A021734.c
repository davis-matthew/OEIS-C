/*
 * A021734  Expansion of 1/((1-x)(1-3x)(1-11x)(1-12x)).
 * https://oeis.org/A021734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 227*a(n-2) + 597*a(n-3) - 396*a(n-4) Initial
 *            values: a(0)=1, a(1)=27, a(2)=502, a(3)=8022
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initialize cache on first call */
    if (cache_cap == 0)
    {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort();          /* OOM */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)502;
        cache[3] = (seq_t)8022;
        cache_len = 4;
    }

    if (n < cache_len)
        return cache[n];

    /* Grow cache if necessary */
    idx_t needed = n + 1;
    if (needed > cache_cap)
    {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();            /* OOM */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        seq_t m1 = (seq_t)27  * cache[k - 1];
        seq_t m2 = (seq_t)227 * cache[k - 2];
        seq_t m3 = (seq_t)597 * cache[k - 3];
        seq_t m4 = (seq_t)396 * cache[k - 4];
        cache[k] = m1 - m2 + m3 - m4;
    }

    cache_len = needed;
    return cache[n];
}
