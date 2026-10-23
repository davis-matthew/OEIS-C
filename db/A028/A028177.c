/*
 * A028177  Expansion of 1/((1-5*x)*(1-6*x)*(1-10*x)*(1-11*x)).
 * https://oeis.org/A028177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028177: Expansion of 1/((1-5*x)*(1-6*x)*(1-10*x)*(1-11*x))
   Recurrence: a(0)=1, a(1)=32, a(2)=653, a(3)=10864,
   a(n)=32*a(n-1)-371*a(n-2)+1840*a(n-3)-3300*a(n-4) for n≥4 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache that keeps computed values for any order of calls */
#ifdef __cplusplus
    static seq_t *cache = nullptr;
#else
    static seq_t *cache = NULL;
#endif
    static idx_t cache_size = 0; /* number of elements currently stored */

    if (n + 1u > cache_size) {
        idx_t new_size = n + 1u;

        /* allocate a new array of the larger size */
#ifdef __cplusplus
        seq_t *newcache = new seq_t[new_size];
#else
        seq_t *newcache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!newcache) abort(); /* avoid compile-time warning due to missing abort include */
#endif

        /* copy the already computed values to the new array */
        for (idx_t i = 0; i < cache_size; ++i) {
            newcache[i] = cache[i];
        }

        /* compute the missing values */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0u) {
                newcache[i] = (seq_t)1;
            } else if (i == 1u) {
                newcache[i] = (seq_t)32;
            } else if (i == 2u) {
                newcache[i] = (seq_t)653;
            } else if (i == 3u) {
                newcache[i] = (seq_t)10864;
            } else {
                /* recurrence for i ≥ 4 */
                /* all intermediate calculations use seq_t to avoid overflow */
                seq_t term = (seq_t)32 * newcache[i - 1u];
                term -= (seq_t)371 * newcache[i - 2u];
                term += (seq_t)1840 * newcache[i - 3u];
                term -= (seq_t)3300 * newcache[i - 4u];
                newcache[i] = term;
            }
        }

        /* free the old cache and replace it with the expanded version */
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
        cache = newcache;
        cache_size = new_size;
    }

    return cache[n];
}
