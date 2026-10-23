/*
 * A192907  Coefficient of x in the reduction by (x^2 -> x + 1) of the
 *          polynomial p(n,x) defined below at Comments.
 * https://oeis.org/A192907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-3) + a(n-4), with initial values a(0)=0,
 *            a(1)=1, a(2)=4, a(3)=12
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised computation of A192907 */
seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of values stored, i.e. next free index */
    static idx_t cache_cap = 0;  /* allocated capacity */

    /* Initialise cache on first call */
    if (cache_size == 0) {
        /* Reserve space for initial 4 values */
        cache_cap = 64;           /* start with a generous capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) {
            /* Out of memory; abort as this is unlikely in tests */
            exit(1);
        }
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        cache_size = 4; /* next free index = 4 */
    }

    /* Grow cache if necessary */
    if ((idx_t)n >= cache_size) {
        /* Ensure capacity */
        idx_t needed = n + 1;
        if (needed > cache_cap) {
            /* Grow strategy: double until enough */
            idx_t new_cap = cache_cap;
            while (new_cap < needed)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing values up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val = (seq_t)3 * cache[i - 1]
                        + cache[i - 3]
                        + cache[i - 4];
            cache[i] = val;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
