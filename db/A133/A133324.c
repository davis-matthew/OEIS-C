/*
 * A133324  7-gonal numbers which are sum of 2 consecutive 7-gonal numbers.
 * https://oeis.org/A133324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 1536796802*a(n-2) - 1536796802*a(n-3) - a(n-4) +
 *            a(n-5); Recurrence is defined for n >= 6 with initial terms:
 *            a(1)=1, a(2)=144841, a(3)=927821665, a(4)=222590743768705,
 *            a(5)=1425873367156486249
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle indices below 1 – not required by OEIS, return 0 */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;   /* cache[0] holds a(1) */
    static idx_t cache_len = 0;    /* number of terms stored */

    /* initialise first 5 terms on first call */
    if (cache == NULL) {
        cache_len = 5;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache)
            return (seq_t)0; /* allocation failure – caller can check */ 
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)144841;
        cache[2] = (seq_t)927821665;
        cache[3] = (seq_t)222590743768705;
        cache[4] = (seq_t)1425873367156486249;
    }

    /* if the requested term is already in cache, return it */
    if (n <= cache_len)
        return cache[n - 1];

    /* enlarge cache to contain up to term n */
    idx_t new_len = n;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!new_cache) {
        /* allocation failure – keep old cache */
        return (seq_t)0;
    }
    cache = new_cache;

    /* compute missing terms using the 5‑term recurrence */
    for (idx_t i = cache_len; i < new_len; ++i) {
        /* i is zero‑based index for term a(i+1) */
        cache[i] = cache[i - 1]                         /* a(n-1) */
                 + 1536796802 * cache[i - 2]            /* +1536796802*a(n-2) */
                 - 1536796802 * cache[i - 3]            /* -1536796802*a(n-3) */
                 - cache[i - 4]                        /* -a(n-4) */
                 + cache[i - 5];                       /* +a(n-5) */
    }
    cache_len = new_len;
    return cache[n - 1];
}
