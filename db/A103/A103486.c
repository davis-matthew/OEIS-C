/*
 * A103486  a(0)=7, a(1)=11, a(2)=13, a(3)=17; then a(n) =
 *          a(n-1)+a(n-3)-a(n-4).
 * https://oeis.org/A103486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=7, a(1)=11, a(2)=13, a(3)=17;
   a(n) = a(n-1) + a(n-3) - a(n-4)   for n ≥ 4   */
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cached sequence values */
static idx_t cache_len = 0;   /* number of values currently cached */
static size_t cache_cap = 0;  /* allocated capacity of cache */

/* grow cache to hold at least (n+1) elements */
static void ensure_cache(idx_t n)
{
    if (n < cache_len)          /* already computed */
        return;

    /* ensure capacity for indices 0..n */
    size_t needed = (size_t)n + 1;
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_buf) /* allocation failure: give up silently */
            return;
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i)
                new_buf[i] = cache[i];
            free(cache);
        }
        cache = new_buf;
        cache_cap = new_cap;
    }

    /* initialise the first four terms if the cache was empty */
    if (cache_len == 0) {
        cache[0] = 7;
        cache[1] = 11;
        cache[2] = 13;
        cache[3] = 17;
        cache_len = 4;
    }

    /* compute missing terms up to n (inclusive) */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
    }

    /* update cache length (never reduce it) */
    if (cache_len < n + 1)
        cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
