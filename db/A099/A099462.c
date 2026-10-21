/*
 * A099462  Expansion of x/(1 - 4*x^2 - 4*x^3).
 * https://oeis.org/A099462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-2) + 4*a(n-3); a(0)=0, a(1)=1, a(2)=0
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 104,
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

    if (cache == NULL) {
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache_len = 3;
    }

    if (n < cache_len)
        return cache[n];

    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)0;
        } else if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)0;
        } else {
            val = (seq_t)4 * cache[i - 2] + (seq_t)4 * cache[i - 3];
        }
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
