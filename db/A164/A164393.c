/*
 * A164393  Number of binary strings of length n with no substrings equal to
 *          0001 or 0011.
 * https://oeis.org/A164393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-4) + a(n-5); * Initial values: * a(0) =
 *            1, a(1) = 2, a(2) = 4, a(3) = 8, a(4) = 14
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;    /* largest index for which cache is valid */

    /* First call: initialise cache with base values up to index 4 */
    if (cache == NULL)
    {
        /* Allocate enough space for at least 5 entries, or more if n is larger
           (use size_t to avoid overflow with very large n) */
        size_t init_size = (size_t)(n + 1);
        if (init_size < 5) init_size = 5;

        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failure */

        /* Base values a(0)..a(4) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)14;
        max_cached = 4;
    }

    /* If the requested index is already cached, return it */
    if (n <= max_cached)
        return cache[n];

    /* Expand the cache to hold up to index n */
    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) exit(1);               /* allocation failure */
    cache = tmp;

    /* Compute missing values using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        /* a(i) = 2*a(i-1) - 2*a(i-4) + a(i-5) */
        seq_t val = (seq_t)2 * cache[i - 1];
        val -= (seq_t)2 * cache[i - 4];
        val += cache[i - 5];
        cache[i] = val;
    }
    max_cached = n;

    return cache[n];
}
