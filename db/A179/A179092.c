/*
 * A179092  Variant of Sylvester's sequence: a(n + 1) = a(n)^2 - a(n) + 1,
 *          with a(1) = 16.
 * https://oeis.org/A179092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n)^2 - a(n) + 1, a(1) = 16
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms cached (a(1) is cache[0]) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    if (n == 0)  /* undefined by OEIS, but guard against it */
        return (seq_t)0;

    /* Ensure enough cache space */
    if (cache_len < n) {
        idx_t needed = n;
        if (cache_cap < needed) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < needed)
                new_cap *= 2;
            seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            if (!new_cache)
                return (seq_t)0;   /* allocation failure - unlikely */
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Initialize first term if cache is empty */
        if (cache_len == 0) {
            cache[0] = (seq_t)16;   /* a(1) */
            cache_len = 1;
        }

        /* Compute up to the requested term */
        for (idx_t i = cache_len; i < n; ++i) {
            seq_t x = cache[i - 1];
            cache[i] = x * x - x + (seq_t)1;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
