/*
 * A155619  a(n) = 8^n+4^n-1^n.
 * https://oeis.org/A155619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + 4^n - 1^n Recurrence: a(0)=1, a(1)=11,
 *            a(n)=12*a(n-1)-32*a(n-2)-21
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoise previously computed terms */
    static seq_t *cache  = NULL;
    static idx_t  cap    = 0;   /* allocated capacity of cache          */
    static idx_t  size   = 0;   /* number of terms already computed     */

    /* initialise the cache on first call */
    if (!cache) {
        cap   = 4;                         /* start with space for a few terms */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;              /* unlikely, but safe */
        size = 0;                          /* nothing yet computed           */
    }

    /* grow cache if the index is beyond the current capacity */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* unlikely, but safe */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms one by one, using the recurrence */
    for (idx_t i = size; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 11;
        } else {
            cache[i] = 12 * cache[i-1] - 32 * cache[i-2] - 21;
        }
    }

    size = n + 1;          /* we now have terms up to index n */
    return cache[n];
}
