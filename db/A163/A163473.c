/*
 * A163473  a(n) = 14*a(n-1) - 46*a(n-2) for n > 1; a(0) = 3, a(1) = 24.
 * https://oeis.org/A163473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 46*a(n-2); initial values: a(0) = 3, a(1) =
 *            24
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of initialized elements */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* ensure the cache holds up to index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if the value is already calculated, return it */
    if (cache_len > (size_t)n)
        return cache[(size_t)n];

    /* fill the cache up to index n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)3;
        } else if (i == 1) {
            cache[i] = (seq_t)24;
        } else {
            seq_t t1 = cache[i - 1];
            seq_t t0 = cache[i - 2];
            cache[i] = (t1 * (seq_t)14) - (t0 * (seq_t)46);
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
