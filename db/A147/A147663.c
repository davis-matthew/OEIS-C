/*
 * A147663  Expansion of 1/(1 - x - x^2 + x^3 - x^7 + x^9 - x^11).
 * https://oeis.org/A147663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 79, u64 n <= 158,
 *            u128 n <= 316, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=3, a(5)=3, a(6)=4,
   a(7)=5, a(8)=7, a(9)=9, a(10)=12,
   a(n)=a(n-1)+a(n-2)-a(n-3)+a(n-7)-a(n-9)+a(n-11) for n>=11 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t computed = 0;   /* next index to compute */

    /* initial values for n = 0..10 */
    static const seq_t base[11] = {
        1, 1, 2, 2, 3, 3, 4, 5, 7, 9, 12
    };

    /* allocate and fill base array on first call */
    if (cache == NULL) {
        size_t init_cap = 16;            /* ensure space for base */
        cap = init_cap;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL)
            return 0; /* allocation failure, return 0 as safe default */
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = base[i];
        computed = 11; /* next index to compute */
    }

    /* return value if already computed */
    if (n < computed)
        return cache[n];

    /* grow cache if necessary */
    if ((idx_t)cap <= n) {
        size_t new_cap = cap;
        while ((idx_t)new_cap <= n)
            new_cap <<= 1;   /* double until large enough */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            free(cache);
            cache = NULL;
            return 0; /* allocation failure */
        }
        cache = new_cache;
        cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t val;
        if (i <= 10) {          /* safety, should not happen after init */
            val = base[i];
        } else {                /* recurrence for i >= 11 */
            val = cache[i-1] + cache[i-2]
                  - cache[i-3]
                  + cache[i-7]
                  - cache[i-9]
                  + cache[i-11];
        }
        cache[i] = val;
    }
    computed = n + 1;

    return cache[n];
}
