/*
 * A075909  Sixth column of triangle A075499.
 * https://oeis.org/A075909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 84*a(n‑1) - 2800*a(n‑2) + 47040*a(n‑3)
 *                 - 415744*a(n‑4) + 1806336*a(n‑5) - 2949120*a(n‑6)
 * with base values:
 *   a(0) = 1
 *   a(1) = 84
 *   a(2) = 4256
 *   a(3) = 169344
 *   a(4) = 5843712
 *   a(5) = 183794688
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* allocate or grow a buffer of seq_t values */
static void grow_cache(seq_t **buf, size_t *cap, size_t need)
{
    if (*cap >= need)
        return;

    size_t new_cap = (*cap > 0) ? (*cap << 1) : 16;
    while (new_cap < need)
        new_cap <<= 1;

    void *mem = malloc(new_cap * sizeof(seq_t));
    if (*buf) {
        memcpy(mem, *buf, (*cap) * sizeof(seq_t));
        free(*buf);
    }
    *buf = (seq_t *)mem;
    *cap = new_cap;
}

/* sequence generator */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* base values */
    static const seq_t base[6] = {
        /*a(0)*/ (seq_t)1,
        /*a(1)*/ (seq_t)84,
        /*a(2)*/ (seq_t)4256,
        /*a(3)*/ (seq_t)169344,
        /*a(4)*/ (seq_t)5843712,
        /*a(5)*/ (seq_t)183794688
    };

    /* initialise cache with base values once */
    if (cache_len == 0) {
        grow_cache(&cache, &cache_cap, 6);
        memcpy(cache, base, 6 * sizeof(seq_t));
        cache_len = 6;
    }

    if (n < cache_len)
        return cache[n];

    /* compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)84   * cache[i-1]
                   - (seq_t)2800 * cache[i-2]
                   + (seq_t)47040 * cache[i-3]
                   - (seq_t)415744 * cache[i-4]
                   + (seq_t)1806336 * cache[i-5]
                   - (seq_t)2949120 * cache[i-6];
        cache[i] = term;
    }

    cache_len = n + 1;
    return cache[(size_t)n];
}
