/*
 * A137787  a(n) = 5^n - 4^n - 3^n - 2^n.
 * https://oeis.org/A137787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137787: a(n) = 5^n - 4^n - 3^n - 2^n
   (also satisfies a(n) = 14*a(n-1)-71*a(n-2)+154*a(n-3)-120*a(n-4) for n>3) */

#include <stddef.h>
#include <stdlib.h>

static seq_t pow_small(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

static void grow_cache(size_t new_len, size_t *cap, seq_t **cache)
{
    if (new_len <= *cap)
        return;
    size_t new_cap = (*cap > 0) ? (*cap << 1) : 4;
    while (new_cap < new_len)
        new_cap <<= 1;
    *cache = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0; /* number of values stored (cache_len = next free index) */

    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    if (n < cache_len)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 4) {
            seq_t p5 = pow_small(5, i);
            seq_t p4 = pow_small(4, i);
            seq_t p3 = pow_small(3, i);
            seq_t p2 = pow_small(2, i);
            val = p5 - p4 - p3 - p2;
        } else {
            /* recurrence: a(n) = 14*a(n-1) - 71*a(n-2) + 154*a(n-3) - 120*a(n-4) */
            val = 14 * cache[i-1]
                - 71 * cache[i-2]
                + 154 * cache[i-3]
                - 120 * cache[i-4];
        }
        grow_cache(i + 1, &cache_cap, &cache);
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
