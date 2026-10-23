/*
 * A022379  Fibonacci sequence beginning 3, 9.
 * https://oeis.org/A022379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = a(n-1) + a(n-2)   with a(0)=3, a(1)=9  (Fibonacci sequence beginning 3,9) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the computed terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;  /* number of valid terms in cache */

    /* already computed? */
    if (n < cache_len)
        return cache[n];

    /* initialise cache for the first two terms */
    if (cache_len == 0) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)9;
        cache_len = 2;
    }

    /* grow capacity if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;  /* double until enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    while (cache_len <= (size_t)n) {
        cache[cache_len] = cache[cache_len - 1] + cache[cache_len - 2];
        ++cache_len;
    }

    return cache[n];
}
