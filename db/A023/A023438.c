/*
 * A023438  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-8).
 * https://oeis.org/A023438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-8) Initial values: a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=8, a(7)=13, a(8)=21
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values up to n=8 */
    if (n <= 8) {
        static const seq_t base[9] = {0, 1, 1, 2, 3, 5, 8, 13, 21};
        return base[n];
    }

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* Number of valid entries in cache */
    static size_t cache_cap = 0;  /* Allocated capacity of cache */

    /* Lazy allocation of cache with initial values (0..8) */
    if (cache == NULL) {
        cache_len = 9;
        cache_cap = 16;  /* start with a small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* allocation failure guard */

        /* load base values into cache */
        static const seq_t base[9] = {0, 1, 1, 2, 3, 5, 8, 13, 21};
        for (size_t i = 0; i < 9; ++i)
            cache[i] = base[i];
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_len) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap <<= 1; /* double until enough */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;  /* allocation failure guard */
        cache = new_cache;
        cache_cap = new_cap;

        /* Compute missing sequence values */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            cache[i] = cache[i-1] + cache[i-2] - cache[i-8];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
