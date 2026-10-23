/*
 * A041269  Denominators of continued fraction convergents to sqrt(147).
 * https://oeis.org/A041269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 194*a(n-2) - a(n-4); a(0)=1, a(1)=8, a(2)=193, a(3)=1552
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values of the sequence */
static size_t cache_len = 0;          /* number of elements currently stored */
static size_t cache_cap = 0;          /* capacity of the cache buffer */

static void ensure_cache(size_t need) {
    if (need < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= need)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the n-th term of OEIS A041269, denominators of continued fraction
   convergents to sqrt(147).  The function is memoised and works for any
   order of calls. */
seq_t a(idx_t n) {
    /* Ensure the cache contains the first four values. */
    if (cache_len == 0) {
        ensure_cache(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)193;
        cache[3] = (seq_t)1552;
        cache_len = 4;
    }

    /* If the requested index is already cached, return it. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to include the requested index. */
    ensure_cache((size_t)n + 1);

    /* Compute missing terms. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1)
            val = (seq_t)8;
        else if (i == 2)
            val = (seq_t)193;
        else if (i == 3)
            val = (seq_t)1552;
        else
            val = 194 * cache[i - 2] - cache[i - 4];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
