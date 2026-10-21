/*
 * A215829  a(n) = -3*a(n-1) + 9*a(n-2) + 3*a(n-3), with a(0)=3, a(1)=-3,
 *          a(2)=27.
 * https://oeis.org/A215829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1) + 9*a(n-2) + 3*a(n-3), a(0) = 3, a(1) = -3,
 *            a(2) = 27
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 13, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* realloc */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)-3;
    if (n == 2) return (seq_t)27;

    /* Static memoisation cache */
    static seq_t *cache = NULL;   /* array of computed values */
    static idx_t cache_size = 0;  /* number of elements currently stored */

    /* If the cache is empty, allocate and initialise the first three terms */
    if (cache_size == 0) {
        /* Need space for indices 0 .. n */
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */

        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-3;
        cache[2] = (seq_t)27;
        cache_size = 3;
    }

    /* If the requested index lies beyond the current cached range,
       extend the cache and fill in the missing values. */
    if (n >= cache_size) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* allocation failure */
        cache = tmp;

        /* Compute terms from cache_size up to n inclusively */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = -3 * cache[i - 1] + 9 * cache[i - 2] + 3 * cache[i - 3];
        }
        cache_size = (idx_t)(new_cap);
    }

    return cache[n];
}
