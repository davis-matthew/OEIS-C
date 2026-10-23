/*
 * A143699  a(n) = 19*a(n-1) - 41*a(n-2) + 19*a(n-3) - a(n-4).
 * https://oeis.org/A143699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 41*a(n-2) + 19*a(n-3) - a(n-4) with a(0)=0,
 *            a(1)=1, a(2)=19, a(3)=319, offset 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL; /* cached values */
static size_t cache_len = 0;/* number of terms stored */
static size_t cache_cap = 0;/* allocated capacity */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n */
    size_t req = (size_t)n + 1;
    if (req > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < req) new_cap = req;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise the first four terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)19;
        cache[3] = (seq_t)319;
        cache_len = 4;
    }

    /* If the requested term is already in cache, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to n inclusive */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t1 = (seq_t)19 * cache[i-1];
        seq_t t2 = (seq_t)41 * cache[i-2];
        seq_t t3 = (seq_t)19 * cache[i-3];
        seq_t t4 = cache[i-4];
        cache[i] = t1 - t2 + t3 - t4;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
