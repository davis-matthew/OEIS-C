/*
 * A092966  Number of interior balls in a truncated tetrahedral arrangement.
 * https://oeis.org/A092966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(n-1)*(23*n^2-19*n+6)
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1039, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4);  a(1)=0,a(2)=10,a(3)=52,a(4)=149 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_max = 0;

    /* Initialize cache with base values */
    if (cache == NULL) {
        cache_size = 8;                     /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return 0; /* allocation failure, return 0 */
        /* zero the whole array for safety */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;

        cache[1] = (seq_t)0;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)52;
        cache[4] = (seq_t)149;
        computed_max = 4;
    }

    /* Return if already computed */
    if (n <= computed_max)
        return cache[n];

    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size;
        while (new_size <= n)
            new_size <<= 1;
        seq_t *temp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!temp) return 0; /* allocation failure */
        cache = temp;
        cache_size = new_size;
        /* initialize new slots to 0 */
        for (idx_t i = computed_max + 1; i < new_size; ++i)
            cache[i] = 0;
    }

    /* Compute terms up to n using the recurrence */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t term = 0;
        term += (seq_t)4 * cache[i - 1];
        term -= (seq_t)6 * cache[i - 2];
        term += (seq_t)4 * cache[i - 3];
        term -= cache[i - 4];
        cache[i] = term;
    }

    computed_max = n;
    return cache[n];
}
