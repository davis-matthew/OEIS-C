/*
 * A190140  a(n) = 2*a(n-1) + a(n-2) + 3*a(n-3), a(0)=1.
 * https://oeis.org/A190140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+a(n-2)+3*a(n-3), a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(idx_t upto)
{
    if (cache_cap > upto)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)upto)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialize base values once */
    if (cache_len == 0) {
        ensure_cache(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)5;
        cache_len = 3;
    }

    if (n < cache_len)
        return cache[n];

    for (idx_t i = cache_len; i <= n; ++i) {
        ensure_cache(i);
        seq_t val = cache[i-1] * (seq_t)2 + cache[i-2] + cache[i-3] * (seq_t)3;
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
