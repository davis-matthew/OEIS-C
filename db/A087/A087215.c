/*
 * A087215  Lucas(6*n): a(n) = 18*a(n-1) - a(n-2), starting with a(0) = 2 and
 *          a(1) = 18.
 * https://oeis.org/A087215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 18, a(n) = 18*a(n-1) - a(n-2) (n ≥ 2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary order of calls */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* ensure cache holds terms up to index n */
    if (n >= cache_len) {
        /* initialise first two terms when cache is empty */
        if (cache_len == 0) {
            cache_len = 2;
            cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
            cache[0] = 2;
            cache[1] = 18;
        }
        /* grow cache to n+1 entries */
        if (n >= cache_len) {
            idx_t new_len = n + 1;
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            for (idx_t i = cache_len; i < new_len; ++i) {
                /* recurrence: a(i) = 18*a(i-1) - a(i-2) */
                cache[i] = 18 * cache[i-1] - cache[i-2];
            }
            cache_len = new_len;
        }
    }
    return cache[n];
}
