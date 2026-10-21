/*
 * A209376  1/4 the number of (n+1) X 3 0..2 arrays with every 2 X 2 subblock
 *          having distinct edge sums.
 * https://oeis.org/A209376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - 4*a(n-4) + 4*a(n-5); a(1)
 *            = 8, a(2) = 17, a(3) = 34, a(4) = 80, a(5) = 170
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_limit = 0;   /* smallest index not yet valid (size of cache entries) */

/* returns A209376(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* we need at least 6 slots to store the base values */
    size_t needed = (size_t)(n + 1);
    if (needed < 6) needed = 6;          /* base values go up to index 5 */

    if (cache_limit < needed) {
        /* grow cache */
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;             /* allocation failure – unlikely */
        cache = tmp;

        if (cache_limit == 0) {         /* first allocation – set base values */
            cache[1] = (seq_t)8;
            cache[2] = (seq_t)17;
            cache[3] = (seq_t)34;
            cache[4] = (seq_t)80;
            cache[5] = (seq_t)170;
        }

        /* compute new values if any */
        idx_t start = (cache_limit >= 6) ? cache_limit : 6;
        for (idx_t i = start; i < (idx_t)needed; ++i) {
            cache[i] = cache[i-1]
                     + (seq_t)6 * cache[i-2]
                     - (seq_t)6 * cache[i-3]
                     - (seq_t)4 * cache[i-4]
                     + (seq_t)4 * cache[i-5];
        }

        cache_limit = (idx_t)needed;
    }

    /* n is guaranteed to be >= 1 and <= cache_limit-1 */
    return cache[n];
}
