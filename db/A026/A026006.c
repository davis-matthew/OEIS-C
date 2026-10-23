/*
 * A026006  Expansion of 1/((1-2x)*(1-5x)*(1-8x)*(1-12x)).
 * https://oeis.org/A026006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 246*a(n-2) + 872*a(n-3) - 960*a(n-4) (derived
 *            from the generating function 1/((1-2x)*(1-5x)*(1-8x)*(1-12x)),
 *            with initial values a(0)=1, a(1)=27, a(2)=483, a(3)=7271)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array holding computed terms */
    static size_t cache_len = 0;     /* number of terms stored (cache_len >= 1) */

    /* ensure cache contains at least the first four terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(4 * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed – unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)483;
        cache[3] = (seq_t)7271;
        cache_len = 4;
    }

    /* extend cache if needed */
    if (n >= (idx_t)cache_len) {
        size_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failed */
        cache = new_cache;

        /* compute new terms using the recurrence */
        for (size_t i = cache_len; i < new_len; ++i) {
            /* use 128‑bit intermediate to avoid overflow on 64‑bit builds */
            unsigned __int128 left  = (unsigned __int128)27 * (unsigned __int128)cache[i-1]
                                   + (unsigned __int128)872 * (unsigned __int128)cache[i-3];
            unsigned __int128 right = (unsigned __int128)246 * (unsigned __int128)cache[i-2]
                                   + (unsigned __int128)960 * (unsigned __int128)cache[i-4];
            cache[i] = (seq_t)(left - right);
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
