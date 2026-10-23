/*
 * A131068  Binomial transform of [1, 1, 7, 7, 7, ...].
 * https://oeis.org/A131068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^n - 6*(n + 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Computed via the linear recurrence a(n)=4*a(n-1)-5*a(n-2)+2*a(n-3) with
   initial values a(0)=1, a(1)=2, a(2)=10, a(3)=32. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoised terms */
    static idx_t cache_len = 0;   /* number of elements actually stored */
    static size_t cache_cap = 0;  /* allocated capacity of cache array */

    /* Initialise cache with the first four terms if not yet done. */
    if (cache_len == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();       /* out–of–memory – terminate */
        cache[0] = (seq_t)1;      /* a(0) */
        cache[1] = (seq_t)2;      /* a(1) */
        cache[2] = (seq_t)10;     /* a(2) */
        cache[3] = (seq_t)32;     /* a(3) */
        cache_len = 4;
    }

    /* If the requested term is already cached, return it immediately. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache array if necessary. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out–of–memory – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate missing terms up to n using the recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = ((seq_t)4) * cache[i-1];
        term += ((seq_t)2) * cache[i-3];
        term -= ((seq_t)5) * cache[i-2];
        cache[i] = term;
    }
    cache_len = n + 1;   /* we now have terms 0..n stored */

    return cache[n];
}
