/*
 * A184189  Half the number of (n+1) X 2 binary arrays with no 2 X 2 subblock
 *          containing exactly one 1.
 * https://oeis.org/A184189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 2*a(n-3) for n>=4; with a(1)=6,
 *            a(2)=19, a(3)=60
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, free */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1 – behaviour irrelevant */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* enlarge cache if necessary */
    if (n >= cache_len) {
        idx_t newlen = n + 1;          /* store up to index n inclusive */
        seq_t *newcache = (seq_t *)malloc(sizeof(seq_t) * newlen);
        if (cache) free(cache);        /* discard old cache */
        cache = newcache;
        cache_len = newlen;

        /* initialise base cases */
        if (cache_len > 0) cache[0] = 0;      /* unused but defined */
        if (cache_len > 1) cache[1] = 6;
        if (cache_len > 2) cache[2] = 19;
        if (cache_len > 3) cache[3] = 60;

        /* compute remaining terms using the recurrence */
        for (idx_t i = 4; i < cache_len; ++i) {
            cache[i] = 4 * cache[i-1] - 2 * cache[i-2] - 2 * cache[i-3];
        }
    }

    return cache[n];
}
