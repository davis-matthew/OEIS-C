/*
 * A042109  Denominators of continued fraction convergents to sqrt(579).
 * https://oeis.org/A042109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 16, a(2) = 769, a(3) = 12320, a(n) = 770*a(n-2)
 *            - a(n-4) for n > 3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache   = NULL;   /* array of computed terms        */
static idx_t cache_sz  = 0;     /* number of terms stored          */
static size_t cache_cap = 0;    /* allocated size of cache         */

seq_t a(idx_t n)
{
    /* initialise cache with the four known terms */
    if (!cache) {
        cache_cap = 8;                         /* initial capacity            */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failure (unlikely) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)769;
        cache[3] = (seq_t)12320;
        cache_sz = 4;
    }

    /* return if already computed */
    if (n < cache_sz)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap <<= 1;        /* double until we have enough space */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all terms up to n (inclusive) */
    for (idx_t i = cache_sz; i <= n; ++i) {
        seq_t val;
        if (i < 4) {           /* should not happen because cache_sz >= 4 */
            if (i == 0) val = (seq_t)1;
            else if (i == 1) val = (seq_t)16;
            else if (i == 2) val = (seq_t)769;
            else            val = (seq_t)12320;
        } else {
            /* recurrence: a(i) = 770*a(i-2) - a(i-4) */
            val = (seq_t)(770 * cache[i - 2]) - cache[i - 4];
        }
        cache[i] = val;
    }
    cache_sz = n + 1;          /* update number of stored terms */

    return cache[n];
}
