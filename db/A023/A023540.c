/*
 * A023540  Expansion of 1/((1-x)(1-5x)(1-9x)(1-11x)).
 * https://oeis.org/A023540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - 224*a(n-2) + 694*a(n-3) - 495*a(n-4) with
 *            a(0)=1, a(1)=26, a(2)=452, a(3)=6622
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with known first four terms */
    if (!cache) {
        cache_size = 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed – unlikely */
        cache[0] = 1;
        cache[1] = 26;
        cache[2] = 452;
        cache[3] = 6622;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_size)
        return cache[n];

    /* extend cache up to the required index using the recurrence */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – unlikely */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t val = 26 * a1 - 224 * a2 + 694 * a3 - 495 * a4;
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
