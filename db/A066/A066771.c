/*
 * A066771  a(n) = 5^n*cos(2*n*arctan(1/2)) or denominator of
 *          tan(2*n*arctan(1/2)).
 * https://oeis.org/A066771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(0)=1, a(1)=3, for n≥2: a(n)=6*a(n-1)−25*a(n-2) (cf. the OGF
 *            (1-3x)/(1-6x+25x²))
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>  /* for malloc and realloc */

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;     /* pointer to term array */
    static idx_t cache_size = 0;    /* current allocated size (number of slots) */
    static idx_t last_computed = 1; /* highest index for which cache[i] is valid */

    /* First call: allocate and initialise the first two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory, unlikely for tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
    }

    /* If the requested index is already in the cache, simply return it */
    if (n <= last_computed)
        return cache[n];

    /* Ensure the cache has enough space for index n */
    if ((idx_t)(n + 1) > cache_size) {
        idx_t new_size = cache_size;
        while ((idx_t)(n + 1) > new_size)
            new_size *= 2;        /* grow by factor of two */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] = 6 * cache[i - 1] - 25 * cache[i - 2];
    }
    last_computed = n;

    return cache[n];
}
