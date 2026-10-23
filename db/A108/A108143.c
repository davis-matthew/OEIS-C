/*
 * A108143  a(n)= 5*a(n-1) -a(n-2) -a(n-3).
 * https://oeis.org/A108143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - a(n-2) - a(n-3) a(0) = 1, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static const seq_t init_vals[3] = { (seq_t)1, (seq_t)1, (seq_t)1 };

seq_t a(idx_t n)
{
    /* Handle the first three base cases directly. */
    if (n < 3)
        return init_vals[n];

    /* Initialise the cache on first use. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) abort();            /* allocation failure */
        for (idx_t i = 0; i < 3; ++i)
            cache[i] = init_vals[i];
    }

    /* Extend the cache up to index n if necessary. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();        /* reallocation failure */
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t v = cache[i-1] * (seq_t)5;
            v -= cache[i-2];
            v -= cache[i-3];
            cache[i] = v;
        }
        cache_len = new_len;
    }

    return cache[n];
}
