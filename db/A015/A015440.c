/*
 * A015440  a(n) = a(n-1) + 5*a(n-2), with a(0) = a(1) = 1.
 * https://oeis.org/A015440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 5*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* dynamic memoization cache */
    static seq_t *cache = NULL;          /* array of computed terms */
    static idx_t  cache_size = 0;        /* current capacity (in terms) */
    static idx_t  last_computed = 1;     /* largest index that has been filled */

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        last_computed = 1;
    }

    /* grow cache if needed */
    if (n >= cache_size) {
        idx_t new_cap = cache_size;
        if (new_cap == 0) new_cap = 2;
        while (new_cap <= n) new_cap <<= 1;  /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_size = new_cap;
    }

    /* compute missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + (seq_t)5 * cache[i-2];
        }
        last_computed = n;
    }

    return cache[n];
}
