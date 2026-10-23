/*
 * A133345  a(n) = 2*a(n-1) + 14*a(n-2) for n>1, a(0)=1, a(1)=1.
 * https://oeis.org/A133345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 14*a(n-2) for n>1, a(0)=1, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static idx_t  cache_max = 0;

static void ensure_alloc(idx_t n)
{
    if (cache_len <= n) {
        idx_t new_len = (cache_len == 0) ? 2 : cache_len;
        while (new_len <= n) new_len <<= 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (cache_max == 0) {
        ensure_alloc(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }

    if (n <= cache_max) return cache[n];

    ensure_alloc(n);
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = ( (seq_t)2 * cache[i-1] ) + ( (seq_t)14 * cache[i-2] );
    }
    cache_max = n;
    return cache[n];
}
