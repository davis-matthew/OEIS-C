/*
 * A091701  Row sums of triangle A091700.
 * https://oeis.org/A091701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+3*a(n-2)-2*a(n-3)-a(n-4); initial values:
 *            a(0)=1, a(1)=3, a(2)=14, a(3)=63
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* protect against overflow of size_t when n is huge */
    if (n < 0) return (seq_t)0;
    size_t sn = (size_t)n;
    grow_cache(sn + 1);
    if (sn < cache_len) return cache[sn];

    for (size_t i = cache_len; i <= sn; ++i)
    {
        seq_t val;
        if (i == 0) val = (seq_t)1;
        else if (i == 1) val = (seq_t)3;
        else if (i == 2) val = (seq_t)14;
        else if (i == 3) val = (seq_t)63;
        else
            val = 4 * cache[i-1] + 3 * cache[i-2] - 2 * cache[i-3] - cache[i-4];
        cache[i] = val;
    }
    cache_len = sn + 1;
    return cache[sn];
}
