/*
 * A121740  Solutions to the Pell equation x^2 - 17y^2 = 1 (y values).
 * https://oeis.org/A121740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*a(n-1) - a(n-2), a(1)=0, a(2)=8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 22,
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

    if (n < 1) return 0;          /* values below offset are undefined */
    if (n == 1) return 0;
    if (n == 2) return 8;

    /* initialise the cache for the first time */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t*)malloc((cache_size + 1) * sizeof(seq_t)); /* index 0 unused */
        if (cache == NULL) return 0;   /* out of memory, unlikely in tests */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 8;
    }

    /* grow the cache if needed */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t*)realloc(cache, (new_size + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* realloc failed, unlikely */
        cache = tmp;
        /* compute missing terms */
        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            cache[i] = (seq_t)66 * cache[i-1] - cache[i-2];
        }
        cache_size = new_size;
    }

    return cache[n];
}
