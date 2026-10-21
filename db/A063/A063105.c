/*
 * A063105  Dimension of the space of weight 2n cusp forms for Gamma_0( 37 ).
 * https://oeis.org/A063105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-2)+a(n-3)-a(n-5) for n>6;
   Initial values: a(1)=2, a(2)=9, a(3)=15, a(4)=21, a(5)=27, a(6)=35 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms (0‑based index corresponds to a(k+1)) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of valid entries stored   */
    static idx_t cache_cap = 0;  /* allocated capacity                */

    if (n == 0)
        return 0;  /* out-of-range, defined as 0 */

    /* initialise cache with the first six values if necessary */
    if (cache_len == 0) {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        cache[0] = 2;   /* a(1) */
        cache[1] = 9;   /* a(2) */
        cache[2] = 15;  /* a(3) */
        cache[3] = 21;  /* a(4) */
        cache[4] = 27;  /* a(5) */
        cache[5] = 35;  /* a(6) */
        cache_len = 6;
    }

    /* return cached value if already computed */
    if (n <= cache_len)
        return cache[n-1];

    /* enlarge cache capacity if required */
    if ((idx_t)n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (idx_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i is 0‑based; a(i+1) = a(i-1) + a(i-2) - a(i-4) */
        cache[i] = cache[i-2] + cache[i-3] - cache[i-5];
    }
    cache_len = n;

    return cache[n-1];
}
