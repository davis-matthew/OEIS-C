/*
 * A181329  Number of 2-compositions of n having no column with an even sum. A
 *          2-composition of n is a nonnegative matrix with two rows, such
 *          that each column has at least one nonzero entry and whose entries
 *          sum up to n.
 * https://oeis.org/A181329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=2, a(2)=4, a(3)=12;
   a(n)=2*a(n-1)+2*a(n-2)-a(n-4)+(n==4?1:0)   for n>=4
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;     /* pointer to cached values */
    static size_t  cache_cap = 0;   /* allocated capacity */
    static size_t  cache_len = 0;   /* number of values stored */

    /* Initialise cache with the first four terms if empty */
    if (cache_len == 0) {
        /* Ensure capacity for at least four entries */
        size_t init_cap = 4;
        if (cache_cap < init_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* allocation failed */
            cache = tmp;
            cache_cap = init_cap;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        cache_len = 4;
    }

    /* If n is already cached, simply return it */
    if (n < (idx_t)cache_len)
        return cache[n];

    /* Extend cache if required */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = ((size_t)(n) + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t rhs = (i == 4) ? (seq_t)1 : (seq_t)0;
        cache[i] = 2 * cache[i - 1] + 2 * cache[i - 2] - cache[i - 4] + rhs;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
