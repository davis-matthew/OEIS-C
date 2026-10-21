/*
 * A088014  Expansion of e.g.f.: cosh(sqrt(2)*x)*(1+exp(x)).
 * https://oeis.org/A088014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 2*a(n-4) Initial values:
 *            a(0)=2, a(1)=1, a(2)=5, a(3)=7
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* cached terms */
static idx_t cache_len = 0;   /* number of terms stored (next free index) */
static size_t cache_cap = 0;  /* allocated capacity in seq_t elements */

/* Compute a(n) for A088014 */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first four terms on the first call */
    if (!cache) {
        cache_cap = 8;                     /* at least 4 */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();               /* unrecoverable error */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)7;
        cache_len = 4;                     /* indices 0..3 cached */
    }

    /* If the requested index is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Extend the cache up to index n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i-1]
                  + (seq_t)3 * cache[i-2]
                  - (seq_t)4 * cache[i-3]
                  - (seq_t)2 * cache[i-4];

        /* Grow the allocation if needed */
        if ( (size_t)i >= cache_cap ) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)i)
                new_cap = (size_t)(i + 1);
#ifdef __cplusplus
            seq_t *tmp = reinterpret_cast<seq_t*>(realloc(cache,
                                                          new_cap * sizeof(seq_t)));
#else
            seq_t *tmp = (seq_t*)realloc(cache,
                                         new_cap * sizeof(seq_t));
#endif
            if (!tmp) abort();               /* unrecoverable error */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[i] = val;
    }

    cache_len = n + 1;                         /* indices 0..n cached */
    return cache[n];
}
