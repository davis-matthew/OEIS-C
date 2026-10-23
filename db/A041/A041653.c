/*
 * A041653  Denominators of continued fraction convergents to sqrt(345).
 * https://oeis.org/A041653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 13522 * a(n-10) – a(n-20) for n > 19 with initial values
 *            a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 5, a(4) = 7, a(5) = 47,
 *            a(6) = 54, a(7) = 155, a(8) = 209, a(9) = 364, a(10) = 13313,
 *            a(11) = 13677, a(12) = 26990, a(13) = 67657, a(14) = 94647,
 *            a(15) = 635539, a(16) = 730186, a(17) = 2095911, a...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[20] = {
    1, 1, 2, 5, 7, 47, 54, 155, 209, 364,
    13313, 13677, 26990, 67657, 94647, 635539,
    730186, 2095911, 2826097, 4922008
};

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n < 20)
        return init_vals[n];

    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of terms stored (max index + 1) */
    static idx_t cache_cap  = 0;      /* allocated capacity */

    /* Allocate cache on first use. */
    if (cache == NULL) {
        cache_cap  = 20000;                 /* enough for n up to ~10 000 */
        cache_size = 20;                   /* we already know 20 terms */
        cache = (seq_t*)malloc(sizeof(seq_t) * (cache_cap + 1));
        if (!cache) return 0; /* out of memory – return zero as sentinel */
        for (idx_t i = 0; i < 20; ++i)
            cache[i] = init_vals[i];
    }

    /* Grow the cache if we need more room. */
    if (n >= cache_cap + 1) {
        idx_t new_cap = n * 2 + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (!new_cache) return 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Extend the computed values up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* i >= 20 here because cache_size starts at 20. */
        cache[i] = (seq_t)13522 * cache[i-10] - cache[i-20];
    }
    cache_size = n + 1;

    return cache[n];
}
