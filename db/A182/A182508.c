/*
 * A182508  a(0)=0, a(1)=1, a(n) = (a(n-2)+a(n-1)+n) AND n.
 * https://oeis.org/A182508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(0)=0, a(1)=1, a(n) = (a(n-2)+a(n-1)+n) & n
 * Range:     u8 n <= 256, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two trivial terms. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static memoisation array. */
    static seq_t *cache      = NULL;    /* cache[i] holds a(i) for i <= cache_max */
    static idx_t cache_max   = 0;       /* highest index that has been computed */
    static size_t cache_cap  = 0;       /* allocated capacity (in seq_t elements) */

    /* Initialise the cache on the first call. */
    if (!cache) {
        /* Give a little extra room so that we grow infrequently. */
        cache_cap = (size_t)(n > 512 ? n + 1 : 512);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();            /* out of memory */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_max = 1;
    }
    /* Ensure the cache is large enough to hold a(n). */
    else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to index n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        cache[i] = (cache[i-2] + cache[i-1] + (seq_t)i) & (seq_t)i;
    }
    cache_max = n;

    return cache[n];
}
