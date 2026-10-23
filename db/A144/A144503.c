/*
 * A144503  Sum of n-th antidiagonal of array in A144502.
 * https://oeis.org/A144503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n-1)*a(n-1) + a(n-2) - 2*(n-2) Initial values: a(0) =
 *            1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;      /* array of computed values          */
    static size_t cache_size = 0;    /* current cache capacity as size_t  */

    /* first call: initialise cache with a(0) and a(1) */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure, return 0 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* grow cache to hold values up to n */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute values from first missing index up to n */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t term = a1 * ((seq_t)(2 * (i - 1)));
        term += a2;
        term -= ((seq_t)(2 * (i - 2)));
        cache[i] = term;
    }

    return cache[n];
}
