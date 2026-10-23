/*
 * A022139  Fibonacci sequence beginning 5, 14.
 * https://oeis.org/A022139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=5, a(1)=14, a(n)=a(n-1)+a(n-2) for n>=2
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)14;
        cache_len = 2;
    }

    if (n < cache_len)
        return cache[n];

    while (cache_len <= n) {
        if (cache_len >= cache_cap) {
            cache_cap = cache_cap * 2;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        cache[cache_len] = cache[cache_len - 1] + cache[cache_len - 2];
        cache_len++;
    }

    return cache[n];
}
