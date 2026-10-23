/*
 * A028170  Expansion of 1/((1-5x)(1-6x)(1-8x)(1-9x)).
 * https://oeis.org/A028170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^(n+3) - 2*8^(n+3) + 2*6^(n+3) - 5^(n+3))/12
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values up to cache_lim */
static size_t cache_cap = 0;  /* allocated array capacity          */
static idx_t cache_lim = (idx_t)-1;   /* highest index for which cache[i] is valid */

/* Ensure cache contains values up to index n (inclusive) */
static void ensure_cache(idx_t n) {
    if (cache == NULL) {
        cache_cap = (n < 4 ? 4 : n + 1u);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return; /* OOM not expected in tests */

        /* initial values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)495;
        cache[3] = (seq_t)7070;
        cache_lim = 3;
        if (n <= 3) return;
    }

    if (cache_lim >= n) return;

    /* grow array if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* compute new entries */
    for (idx_t i = cache_lim + 1; i <= n; ++i) {
        /* recurrence: a(n) = 28*a(n-1) - 289*a(n-2) + 1302*a(n-3) - 2160*a(n-4) */
        seq_t t1 = cache[i - 1] * (seq_t)28;
        seq_t t2 = cache[i - 3] * (seq_t)1302;
        t1 = t1 + t2;

        t2 = cache[i - 2] * (seq_t)289;
        seq_t t3 = cache[i - 4] * (seq_t)2160;
        t2 = t2 + t3;

        cache[i] = t1 - t2;
    }

    cache_lim = n;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}
