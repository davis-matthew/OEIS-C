/*
 * A084134  a(n) = 8*a(n-1) - 6*a(n-2), a(0) = 1, a(1) = 4.
 * https://oeis.org/A084134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 6*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoization cache */
    static seq_t *cache   = NULL;   /* array holding the computed terms */
    static size_t  cap    = 0;      /* allocated capacity of cache  */
    static size_t  max_n  = 0;      /* largest n for which cache contains a value */

    /* Directly handle the two seed values. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    /* First call: allocate the cache and store the seeds. */
    if (cache == NULL) {
        cap  = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        max_n   = 1;
    }

    /* Grow the cache until it can hold index n. */
    while (cap <= (size_t)n) {
        size_t new_cap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)8 * cache[i - 1] - (seq_t)6 * cache[i - 2];
    }
    max_n = (size_t)n;

    return cache[n];
}
