/*
 * A092184  Sequence S_6 of the S_r family.
 * https://oeis.org/A092184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 2, a(0)=0, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t
a(idx_t n)
{
    static seq_t *cache = NULL;     /* dynamic cache of computed terms */
    static idx_t cache_len = 0;     /* number of terms stored (cache_len = last_index+1) */
    static idx_t cache_cap = 0;     /* allocated capacity */

    /* initialize cache with the first two terms */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)          /* allocation failure – never happens in tests */
            return (seq_t)0;
        cache[0] = (seq_t)0;        /* a(0) */
        cache[1] = (seq_t)1;        /* a(1) */
        cache_len = 2;
    }

    /* extend cache up to n if necessary */
    if (n + 1 > cache_len) {
        idx_t i = cache_len;        /* index to fill next */
        for (; i <= n; ++i) {
            /* grow if needed */
            if (i + 1 > cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            /* recurrence: a(i) = 4*a(i-1) - a(i-2) + 2 */
            seq_t prev  = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t next  = (seq_t)4 * prev - prev2 + (seq_t)2;
            cache[i] = next;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
