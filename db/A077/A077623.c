/*
 * A077623  a(1)=1, a(2)=2, a(3)=4, a(n) = abs(a(n-1) - a(n-2) - a(n-3)).
 * https://oeis.org/A077623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(a(n-1) - a(n-2) - a(n-3)), with a(1)=1, a(2)=2,
 *            a(3)=4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static void grow_cache(size_t needed, seq_t **cache, size_t *cap) {
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    *cache = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n <= cache_len)
        return cache[n - 1];

    if (cache_len == 0) {
        grow_cache(3, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache_len = 3;
    }

    if (n <= cache_len)
        return cache[n - 1];

    grow_cache((size_t)n, &cache, &cache_cap);
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t sum23 = a2 + a3;
        seq_t diff;
        if (a1 >= sum23)
            diff = a1 - sum23;
        else
            diff = sum23 - a1;
        cache[i] = diff;
    }
    cache_len = n;
    return cache[n - 1];
}
