/*
 * A042408  Numerators of continued fraction convergents to sqrt(732).
 * https://oeis.org/A042408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(n) = 974*a(n-2) - a(n-4),
   initial values: a(0)=27, a(1)=487, a(2)=26325, a(3)=474337 */

seq_t a(idx_t n) {
    /* Handle explicit initial terms */
    if (n == 0) return (seq_t)27;
    if (n == 1) return (seq_t)487;
    if (n == 2) return (seq_t)26325;
    if (n == 3) return (seq_t)474337;

    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    idx_t old_size;                      /* number of terms already present */

    /* Initialise cache if it does not exist yet */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)27;
        cache[1] = (seq_t)487;
        cache[2] = (seq_t)26325;
        cache[3] = (seq_t)474337;
        old_size = 4;
    } else {
        old_size = cache_size;

        /* Grow the cache if necessary */
        if (n + 1 > cache_size) {
            idx_t new_size = n + 1;
            seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!new_cache) abort();
            for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cache_size = new_size;
        }
    }

    /* Compute any terms that are still missing */
    for (idx_t i = old_size; i <= n; ++i) {
        cache[i] = (seq_t)974 * cache[i - 2] - cache[i - 4];
    }

    return cache[n];
}
