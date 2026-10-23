/*
 * A140498  a(n) = 3*a(n-1)-3*a(n-2)+3*a(n-3) with a(0)=1, a(1)=3, a(2)=7.
 * https://oeis.org/A140498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)-3*a(n-2)+3*a(n-3), with a(0)=1, a(1)=3, a(2)=7
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (needed < *cap) return;
    size_t newcap = (*cap == 0) ? 4 : *cap * 2;
    while (newcap <= needed) newcap *= 2;
    *ptr = (seq_t *)realloc(*ptr, newcap * sizeof(seq_t));
    *cap = newcap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;  /* number of valid entries in cache */

    /* Ensure cache can hold up to index n */
    ensure_capacity((size_t)n + 1, &cache_cap, &cache);

    /* Lazy initialization of first three terms */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)7;
        cache_len = 3;
    }

    if (n < cache_len) return cache[n];

    /* Compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1]
                 - (seq_t)3 * cache[i-2]
                 + (seq_t)3 * cache[i-3];
    }
    cache_len = n + 1;
    return cache[n];
}
