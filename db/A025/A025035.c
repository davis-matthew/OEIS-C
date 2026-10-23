/*
 * A025035  Number of partitions of { 1, 2, ..., 3n } into sets of size 3.
 * https://oeis.org/A025035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n)!/(n!*6^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Recurrence: a(0)=1; for n>=1, a(n) = ((3*n-1)*(3*n-2)/2)*a(n-1) */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with a(0) */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* grow cache up to index n if needed */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failure */
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t1 = (seq_t)3 * (seq_t)i - (seq_t)1;   /* 3*i - 1 */
            seq_t t2 = (seq_t)3 * (seq_t)i - (seq_t)2;   /* 3*i - 2 */
            seq_t prod = t1 * t2;                       /* (3*i-1)*(3*i-2) */
            prod = prod / (seq_t)2;                     /* divide by 2 */
            cache[i] = prod * cache[i - 1];             /* recurrence */
        }
        cache_len = new_len;
    }

    return cache[n];
}
