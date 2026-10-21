/*
 * A094659  Number of closed walks of length n at a vertex of the cyclic graph
 *          on 7 nodes C_7.
 * https://oeis.org/A094659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 3*a(n-3) - 2*a(n-4) a(0)=1, a(1)=0,
 *            a(2)=2, a(3)=0 Generates The OEIS A094659 sequence
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed terms. */
    static seq_t *cache      = NULL;
    static size_t cached_len = 0;   /* number of terms already filled */
    static size_t cached_cap = 0;   /* capacity of the cache array      */

    /* Ensure capacity to hold term n.  Double capacity as needed. */
    if (n >= cached_cap) {
        size_t new_cap = (cached_cap == 0) ? 8 : cached_cap * 2;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cached_cap = new_cap;
    }

    /* Initialise first four terms if not yet done. */
    if (cached_len == 0) {
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)0;   /* a(1) */
        cache[2] = (seq_t)2;   /* a(2) */
        cache[3] = (seq_t)0;   /* a(3) */
        cached_len = 4;
    }

    /* If requested term already computed, return it directly. */
    if (n < cached_len)
        return cache[n];

    /* Compute missing terms up to n. */
    for (size_t i = cached_len; i <= n; ++i) {
        /* Recurrence: a(i) = a(i-1) + 4*a(i-2) - 3*a(i-3) - 2*a(i-4) */
        seq_t val =
            cache[i - 1] +
            (seq_t)4 * cache[i - 2] -
            (seq_t)3 * cache[i - 3] -
            (seq_t)2 * cache[i - 4];
        cache[i] = val;
    }
    cached_len = n + 1;

    return cache[n];
}
