/*
 * A041661  Denominators of continued fraction convergents to sqrt(349).
 * https://oeis.org/A041661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 18420*a(n-7) + a(n-14) for n>13 Initial values (n=0..13):
 *            1, 1, 3, 22, 157, 336, 493, 18084, 18577, 55238, 405243,
 *            2891939, 6189121, 9081060
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* memoized values */
    static idx_t cache_size = 0;     /* number of values currently stored */
    static idx_t cache_capacity = 0; /* allocated capacity */

    /* initial seed values */
    static const seq_t init[14] = {
        1ULL, 1ULL, 3ULL, 22ULL, 157ULL, 336ULL, 493ULL,
        18084ULL, 18577ULL, 55238ULL, 405243ULL, 2891939ULL,
        6189121ULL, 9081060ULL
    };

    if (n >= cache_size) {
        /* extend allocation if needed */
        idx_t new_cap = n + 1;
        if (new_cap > cache_capacity) {
            idx_t cand = cache_capacity ? cache_capacity * 2 : 16;
            if (cand < new_cap) cand = new_cap;
            seq_t *tmp = (seq_t *)realloc(cache, cand * sizeof(seq_t));
            cache = tmp;
            cache_capacity = cand;
        }

        /* compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 14) {
                cache[i] = init[i];
            } else {
                cache[i] = 18420 * cache[i-7] + cache[i-14];
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
