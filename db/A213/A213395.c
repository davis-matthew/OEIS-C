/*
 * A213395  Number of (w,x,y) with all terms in {0,...,n} and max(|w-x|,|x-y|)
 *          = w.
 * https://oeis.org/A213395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   recurrence a(0) = 1, a(1) = 4, a(2) = 11, a(3) = 19, a(4) = 31,
 *            a(5) = 44, a(6) = 62 a(n) = 2*a(n-2) + a(n-3) - a(n-4) -
 *            2*a(n-5) + a(n-7) for n ≥ 7
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t last = (idx_t)-1;      /* last index that has been computed   */
    static size_t cap   = 0;            /* current capacity of the cache array */

    /* initialise the cache to hold the first 7 values */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – unlikely in tests */

        cache[0] = 1; cache[1] = 4; cache[2] = 11; cache[3] = 19;
        cache[4] = 31; cache[5] = 44; cache[6] = 62;
        last = 6;
    }

    /* grow the cache if the requested index is beyond the current range */
    if (n > last) {
        /* ensure sufficient capacity */
        if ((idx_t)cap <= n) {
            size_t newcap = cap;
            while ((idx_t)newcap <= n) newcap <<= 1;
            cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!cache) return (seq_t)0;   /* allocation failure – unlikely */
            cap = newcap;
        }
        /* compute missing terms
           recurrence uses values at i-2, i-3, i-4, i-5, i-7, which are
           all already available for i ≥ 7 and i > last. */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t val = 2 * cache[i - 2] + cache[i - 3]
                      - cache[i - 4] - 2 * cache[i - 5] + cache[i - 7];
            cache[i] = val;
        }
        last = n;
    }

    /* for n < 7 the value was pre‑filled during initialisation */
    return cache[n];
}
