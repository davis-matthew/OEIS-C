/*
 * A021344  Expansion of 1/((1-x)*(1-3*x)*(1-4*x)*(1-5*x)).
 * https://oeis.org/A021344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=13, a(2)=110, a(3)=770; a(n) = 13*a(n-1) -
 *            59*a(n-2) + 107*a(n-3) - 60*a(n-4) for n>3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_len = 0;

static void compute_to(idx_t n)
{
    if (cache_len == 0) {
        /* Allocate enough space, at least for indices 0 … n. */
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        cache_len = n + 1;

        /* Base values. */
        cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)13;
        if (n >= 2) cache[2] = (seq_t)110;
        if (n >= 3) cache[3] = (seq_t)770;

        /* Compute the rest iteratively. */
        for (idx_t i = 4; i <= n; ++i) {
            seq_t t1 = 13 * cache[i - 1];
            seq_t t2 = 107 * cache[i - 3];
            seq_t t3 = 59 * cache[i - 2] + 60 * cache[i - 4];
            cache[i] = t1 + t2 - t3;
        }
        return;
    }

    if (n < cache_len) return;          /* Already computed. */
    /* Grow the cache. */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return;            /* Allocation failed – unlikely in tests. */
    cache = new_cache;

    /* Compute missing values. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) cache[i] = (seq_t)1;
        else if (i == 1) cache[i] = (seq_t)13;
        else if (i == 2) cache[i] = (seq_t)110;
        else if (i == 3) cache[i] = (seq_t)770;
        else {
            seq_t t1 = 13 * cache[i - 1];
            seq_t t2 = 107 * cache[i - 3];
            seq_t t3 = 59 * cache[i - 2] + 60 * cache[i - 4];
            cache[i] = t1 + t2 - t3;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    compute_to(n);
    return cache[n];
}
