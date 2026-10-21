/*
 * A224405  Number of n X 4 0..1 arrays with rows unimodal and antidiagonals
 *          nondecreasing.
 * https://oeis.org/A224405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) + 2*a(n-3) + 4*a(n-4), a(1) = 11,
 *            a(2) = 56, a(3) = 228, a(4) = 884
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* allocated length of cache */
    static idx_t  max_n   = 0;        /* largest n computed so far */

    if (n == 0)                      /* undefined for n<1 */
        return (seq_t)0;

    /* Ensure that the cache has room for all terms up to n. */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {                  /* allocation failed – clean up */
            free(cache);
            cache = NULL;
            cache_len = 0;
            max_n = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* If the value is already computed, return it promptly. */
    if (n <= max_n)
        return cache[n - 1];

    /* Compute all missing terms up to n. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t val;
        if (k == 1)
            val = (seq_t)11;
        else if (k == 2)
            val = (seq_t)56;
        else if (k == 3)
            val = (seq_t)228;
        else if (k == 4)
            val = (seq_t)884;
        else
            val = (seq_t)5 * cache[k - 2] -
                  (seq_t)5 * cache[k - 3] +
                  (seq_t)2 * cache[k - 4] +
                  (seq_t)4 * cache[k - 5];
        cache[k - 1] = val;
    }

    max_n = n;
    return cache[n - 1];
}
