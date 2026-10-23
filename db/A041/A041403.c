/*
 * A041403  Denominators of continued fraction convergents to sqrt(216).
 * https://oeis.org/A041403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 970*a(n-6) - a(n-12) for n>11; a(0)=1, a(1)=1, a(2)=3,
 *            a(3)=10, a(4)=23, a(5)=33, a(6)=947, a(7)=980, a(8)=2907,
 *            a(9)=9701, a(10)=22309, a(11)=32010
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial twelve terms */
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)10,
        (seq_t)23, (seq_t)33, (seq_t)947, (seq_t)980,
        (seq_t)2907, (seq_t)9701, (seq_t)22309, (seq_t)32010
    };

    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t sz   = 0;   /* number of elements stored */
    static idx_t cap  = 0;   /* allocated capacity */

    /* handle small indices directly */
    if (n < 12)
        return init[n];

    /* initialise cache on first use */
    if (cache == NULL) {
        cap = 12;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)   /* allocation failed – give up */
            return (seq_t)0;
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        sz = 12;
    }

    /* grow cache if needed */
    if (n + 1 > sz) {
        /* increase capacity at least to n+1 */
        if (cap < n + 1) {
            idx_t new_cap = cap;
            while (new_cap < n + 1)
                new_cap = new_cap * 2 + 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)   /* allocation failed – give up */
                return (seq_t)0;
            cache = tmp;
            cap   = new_cap;
        }
        /* compute missing terms */
        for (idx_t i = sz; i <= n; ++i) {
            /* recurrence: a(i) = 970*a(i-6) - a(i-12) */
            seq_t t = ((seq_t)970) * cache[i - 6];
            cache[i] = t - cache[i - 12];
        }
        sz = n + 1;
    }

    return cache[n];
}
