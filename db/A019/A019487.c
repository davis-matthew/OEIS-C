/*
 * A019487  a(n) = 3*a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4).
 * https://oeis.org/A019487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) + 2*a(n-3) - 2*a(n-4); Initial values:
 *            a(0)=2, a(1)=6, a(2)=17, a(3)=48
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed values */
    static seq_t *cache   = NULL;   /* pointer to cache array          */
    static size_t  cap    = 0;      /* current allocated capacity      */
    static idx_t   last   = 3;      /* index of last computed term     */

    /* For n < 4 just return hard‑coded initial values */
    if (n < 4) {
        static const seq_t init[4] = {
            (seq_t)2,  /* a(0) */
            (seq_t)6,  /* a(1) */
            (seq_t)17, /* a(2) */
            (seq_t)48  /* a(3) */
        };
        return init[n];
    }

    /* First call: allocate cache and fill initial terms */
    if (cache == NULL) {
        size_t init_cap = 4;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) abort();          /* memory allocation failed */
        cap = init_cap;
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)48;
    }

    /* Grow storage if necessary           */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 4;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n      */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = 3 * cache[i - 1] - cache[i - 2] + 2 * cache[i - 3] - 2 * cache[i - 4];
        }
        last = n;
    }

    return cache[n];
}
