/*
 * A212252  Number of (w,x,y,z) with all terms in {1,...,n} and 3w=x+y+z+n+2.
 * https://oeis.org/A212252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)-3*a(n-2)+2*a(n-3)-3*a(n-4)+3*a(n-5)-a(n-6)
 *            Initial values: a(0)=0, a(1)=0, a(2)=0, a(3)=3, a(4)=11, a(5)=24
 * Range:     u8 n <= 10, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cached values */
    static idx_t cache_size = 0;         /* allocated size of cache */
    static idx_t computed_max = 0;       /* highest index computed */

    /* Allocate or enlarge cache as needed */
    if (cache == NULL) {
        cache_size = (n < 6 ? 6 : n + 1);
        cache = (seq_t *)realloc(NULL, sizeof(seq_t) * cache_size);

        /* initialise the first six terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)24;
        computed_max = 5;
    } else if (cache_size < n + 1) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);
        /* The new positions are uninitialised; the base values are already set. */
    }

    /* Compute missing terms up to n */
    while (computed_max < n) {
        idx_t k = computed_max + 1;
        if (k <= 5) {
            /* base entries already defined */
        } else {
            seq_t s1 = (seq_t)3 * cache[k-1];
            s1 += (seq_t)2 * cache[k-3];
            s1 += (seq_t)3 * cache[k-5];
            seq_t s2 = (seq_t)3 * cache[k-2];
            s2 += (seq_t)3 * cache[k-4];
            s2 += cache[k-6];
            cache[k] = s1 - s2;
        }
        computed_max = k;
    }

    return cache[n];
}
