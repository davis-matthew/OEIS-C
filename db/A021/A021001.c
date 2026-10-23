/*
 * A021001  Pisot sequence P(2,9).
 * https://oeis.org/A021001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 2, a(1) = 9, a(n) = 4*a(n-1) + 2*a(n-2) (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of terms currently stored */
static size_t cache_cap = 0;     /* capacity of the allocated array */

/* Ensure the cache is large enough to hold at least 'needed' terms. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 2;                        /* we know a(0), a(1) */
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)9;
        cache_len = 2;
        cache_cap = init_cap;
    }

    /* return early if already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* make sure the array is large enough */
    size_t need = (size_t)n + 1;                    /* need up to a(n) */
    ensure_capacity(need);

    /* compute missing terms using the recurrence */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)4 + cache[i - 2] * (seq_t)2;
    }

    cache_len = need;
    return cache[n];
}
