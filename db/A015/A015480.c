/*
 * A015480  q-Fibonacci numbers for q=8, scaling a(n-1).
 * https://oeis.org/A015480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^(n-1)*a(n-1) + a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values and helper variables */
    static seq_t *cache      = NULL;      /* array holding computed terms   */
    static size_t cache_size = 0;         /* number of terms stored (>=1)   */
    static size_t cap        = 0;         /* allocated capacity              */
    static seq_t pow8_next   = 0;         /* 8^(next_index-1)                */

    /* Initial allocation for a(0) and a(1) */
    if (cache_size == 0) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;             /* out‑of‑memory – unlikely in tests */
        cache[0] = 0;
        cache[1] = 1;
        cache_size = 2;                   /* indices 0 and 1 are now stored   */
        pow8_next = (seq_t)8;             /* 8^(2-1) for the first step      */
    }

    /* If the requested term is already cached, return it */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Resize the cache array if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t new_cap = cap;
        while (new_cap < needed)
            new_cap <<= 1;                /* grow by factor of two */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed           */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute the missing terms in sequence order */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* a(i) = 8^(i-1) * a(i-1) + a(i-2)   */
        seq_t term = pow8_next * cache[i-1] + cache[i-2];
        cache[i] = term;

        /* Update pow8_next for the next iteration:
           pow8_next = 8^(i)          */
        pow8_next = pow8_next * (seq_t)8;
    }

    /* Update the size of the cached region */
    cache_size = n + 1;

    return cache[n];
}
