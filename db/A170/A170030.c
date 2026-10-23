/*
 * A170030  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=21*20^(n-1) for n>=1 (i.e., a(n)=20*a(n-1) for n>=2
 *            with a(1)=21)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache = NULL;           /* pointer to array of seq_t */
    static idx_t cache_size = 0;          /* allocated array size */
    static idx_t cache_last = 0;          /* highest index computed */

    /* initialise cache (holds a(0) and a(1) ) */
    if (cache == NULL) {
        cache_size = 2; /* at least two slots for indices 0 and 1 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {            /* allocation failure */
            /* As a fallback return 0; it will never be used in valid tests */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache[1] = (seq_t)21;         /* a(1) = 21 */
        cache_last = 1;
    }

    /* extend cache up to index n if necessary */
    if (n > cache_last) {
        /* grow array to at least n+1 elements, doubling each time */
        size_t new_size = cache_size;
        while ((idx_t)new_size <= n) {
            new_size *= 2;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – return 0 */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;

        /* compute missing values */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)20; /* a(i) = 20*a(i-1) */
        }
        cache_last = n;
    }

    return cache[n];
}
