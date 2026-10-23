/*
 * A009955  Coordination sequence for FeS2-Marcasite, Fe position.
 * https://oeis.org/A009955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A009955 (FeS2‑Marcasite, Fe position)
   Initial values:
   a(0)=1, a(1)=6, a(2)=16, a(3)=40, a(4)=60,
   a(5)=106, a(6)=134, a(7)=206, a(8)=236
   For n>8:
   a(n) = -a(n-1) + 2*a(n-2) + 3*a(n-3)
          - 3*a(n-5) - 2*a(n-6) + a(n-7) + a(n-8) */
#include <stdlib.h>

static const seq_t init[9] = {1, 6, 16, 40, 60, 106, 134, 206, 236};

seq_t a(idx_t n)
{
    /* Return small n directly */
    if (n < 9)
        return init[(size_t)n];

    /* Dynamic cache with static state */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of valid entries (<= cache_cap) */
    static size_t cache_cap   = 0;  /* allocated capacity */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap   = 16;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failure; caller can check for zero */
        for (size_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cache_size = 9;
    }

    /* Ensure enough capacity */
    if ((size_t)n >= cache_cap) {
        size_t newcap = cache_cap;
        while ((size_t)n >= newcap)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)
            return 0; /* allocation failure */
        cache = tmp;
        cache_cap = newcap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        __int128 val = -(__int128)cache[i-1]
                       + 2 * (__int128)cache[i-2]
                       + 3 * (__int128)cache[i-3]
                       - 3 * (__int128)cache[i-5]
                       - 2 * (__int128)cache[i-6]
                       + (__int128)cache[i-7]
                       + (__int128)cache[i-8];
        cache[i] = (seq_t)val;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
