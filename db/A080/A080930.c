/*
 * A080930  a(n) = 2^(n-3)*(n+2)*(n+3)*(n+4)/3.
 * https://oeis.org/A080930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=20, a(3)=70, a(n) = 8*a(n-1) - 24*a(n-2) +
 *            32*a(n-3) - 16*a(n-4) for n>3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 22, u64 n <= 51, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of cached terms */

    /* ensure the cache has size (n+1) and fill missing values */
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort in this test environment */
            exit(1);
        }
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)5;
            } else if (i == 2) {
                cache[i] = (seq_t)20;
            } else if (i == 3) {
                cache[i] = (seq_t)70;
            } else {
                /* recurrence: a(n) = 8*a(n-1) – 24*a(n-2) + 32*a(n-3) – 16*a(n-4) */
                cache[i] = cache[i-1] * (seq_t)8
                         - cache[i-2] * (seq_t)24
                         + cache[i-3] * (seq_t)32
                         - cache[i-4] * (seq_t)16;
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
