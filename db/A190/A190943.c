/*
 * A190943  a(n) = 8*a(n-1) + 27*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 27*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    /* Allocate or extend cache */
    size_t new_len = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        /* Initialize first two terms if needed */
        cache[0] = (seq_t)0;
        if (new_len > 1)
            cache[1] = (seq_t)1;
        cache_len = new_len > 1 ? 2 : 1;
    } else {
        /* Grow array */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL)
            return; /* allocation failed; caller will handle */
        cache = new_cache;
        /* cache_len already >= 2, so nothing else to init */
        cache_len = (size_t)cache_len;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)8 * cache[i - 1] + (seq_t)27 * cache[i - 2];
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    ensure_cache(n);
    return cache[n];
}
