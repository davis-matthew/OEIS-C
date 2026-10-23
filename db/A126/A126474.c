/*
 * A126474  Number of arrays in [1..6]^n with adjacent elements differing by
 *          three or less.
 * https://oeis.org/A126474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5*a(n-1) + a(n-2) - 2*a(n-3) for n>2; initial values:
 *            a(0)=1, a(1)=6, a(2)=30
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[0..max_n] stored */
    static seq_t *cache = NULL;
    static size_t cap   = 0;           /* allocated size of cache */
    static size_t max_n = (size_t)-1;  /* maximum index computed */

    /* first call: allocate and initialise */
    if (max_n == (size_t)-1) {
        /* start with some capacity */
        cap = 128;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) {            /* allocation failure – return 0 */
            return (seq_t)0;
        }
        /* initial three values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)30;
        max_n = 2;
    }

    /* if requested value already computed, just return it */
    if ((size_t)n <= max_n) {
        return cache[(size_t)n];
    }

    /* enlarge the allocation if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap <= (size_t)n + 1) {
            newcap *= 2;
        }
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {                /* allocation failure – return 0 */
            free(cache);
            cache = NULL;
            return (seq_t)0;
        }
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms up to n */
    for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)6;
        } else if (i == 2) {
            cache[2] = (seq_t)30;
        } else {
            cache[i] = 5 * cache[i-1] + cache[i-2] - 2 * cache[i-3];
        }
    }
    max_n = (size_t)n;
    return cache[(size_t)n];
}
