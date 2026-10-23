/*
 * A156626  a(0)=1; a(1)=2; a(2)=6; a(n+1) = 2*(n+1)*a(n) - n^2*a(n-1), n > 1.
 * https://oeis.org/A156626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(1)=2; a(2)=6; a(n+1)=2*(n+1)*a(n)-n*n*a(n-1) (n>1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External type definitions are supplied by the build:
   seq_t   -- unsigned integer type (possibly arbitrary‑precision)
   idx_t   -- unsigned integer type for indices (uint64_t)
*/

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;   /* array of computed terms */
    static size_t cache_len = 0;    /* number of valid entries in cache */
    static size_t cache_cap = 0;    /* allocated capacity of cache */

    /* Grow cache if needed */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 16;
        while (new_cap <= n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to index n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)6;
        } else {
            /* recurrence: a(i) = 2*i*a(i-1) - (i-1)^2 * a(i-2) */
            seq_t left  = (seq_t)2 * (seq_t)i * cache[i - 1];
            seq_t right = (seq_t)(i - 1) * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = left - right;
        }
    }
    cache_len = n + 1;
    return cache[n];
}
