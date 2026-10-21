/*
 * A215004  a(0) = a(1) = 1; for n>1, a(n) = a(n-1) + a(n-2) + floor(n/2).
 * https://oeis.org/A215004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A215004:  a(0)=a(1)=1;
   for n>1, a(n)=a(n-1)+a(n-2)+floor(n/2)  (n is unsigned) */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 1;

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache on first use */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
    }

    /* extend cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc((size_t)new_len * sizeof(seq_t));

        /* copy existing values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            new_cache[i] = new_cache[i-1] + new_cache[i-2] + (seq_t)(i / 2);
        }

        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
