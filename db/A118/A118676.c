/*
 * A118676  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+79)^2 = y^2.
 * https://oeis.org/A118676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3) - a(n-6) + 158 for n > 6; initial values:
 *            a(1)=0, a(2)=20, a(3)=161, a(4)=237, a(5)=341, a(6)=1140
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 32, u64 n <= 69, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to allow calls in any order. */
    static seq_t *cache = NULL;       /* cache[0] holds a(1) */
    static size_t cache_cap = 0;      /* allocated length */
    static idx_t   cache_max = 0;     /* largest n computed so far */

    /* Allocate initial space for first 6 terms, if not yet done. */
    if (cache == NULL) {
        cache_cap = 64;            /* initial capacity, will grow as needed */
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory: return 0 (unlikely in tests) */

        /* Fill the first 6 terms. */
        cache[0] = (seq_t)0;    /* a(1) */
        cache[1] = (seq_t)20;   /* a(2) */
        cache[2] = (seq_t)161;  /* a(3) */
        cache[3] = (seq_t)237;  /* a(4) */
        cache[4] = (seq_t)341;  /* a(5) */
        cache[5] = (seq_t)1140; /* a(6) */

        cache_max = 6;
    }

    /* Grow cache if necessary to accommodate n. */
    if ((idx_t)cache_cap < n) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t val;
        if (k <= 6) {
            /* already filled */
            val = cache[k-1];
        } else {
            /* recurrence: a(k) = 6*a(k-3) - a(k-6) + 158 */
            val = 6 * cache[k-4]       /* a(k-3) at index k-4 */
                  - cache[k-7]         /* a(k-6) at index k-7 */
                  + (seq_t)158;
        }
        cache[k-1] = val;
    }
    cache_max = n > cache_max ? n : cache_max;

    return cache[n-1];
}
