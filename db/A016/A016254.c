/*
 * A016254  Expansion of 1/((1-x)(1-7x)(1-11x)).
 * https://oeis.org/A016254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=19, a(n)=18*a(n-1)-77*a(n-2)+1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 1;   /* highest index computed so far */

    if (!cache) {                     /* first call – initialise cache */
        cache_size = (n > 1) ? (n + 1) : 2;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
        computed = 1;

        if (n == 0) return cache[0];
        if (n == 1) return cache[1];

        /* compute up to n */
        for (idx_t i = 2; i <= n; ++i) {
            cache[i] = (seq_t)18 * cache[i-1]
                     - (seq_t)77 * cache[i-2]
                     + (seq_t)1;
        }
        computed = n;
        return cache[n];
    }

    /* Return cached value if already computed */
    if (n <= computed) return cache[n];

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = (cache_size > 1) ? cache_size : 2;
        while (new_size <= n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)18 * cache[i-1]
                 - (seq_t)77 * cache[i-2]
                 + (seq_t)1;
    }
    computed = n;
    return cache[n];
}
