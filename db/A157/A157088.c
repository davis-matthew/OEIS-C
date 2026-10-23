/*
 * A157088  Consider all consecutive integer Pythagorean septuples (X, X+1,
 *          X+2, X+3, Z-2, Z-1, Z) ordered by increasing Z; sequence gives X
 *          values.
 * https://oeis.org/A157088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 21; for n > 1, a(n) = 14*a(n-1) - a(n-2) + 18
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache    = NULL;   /* array of computed terms   */
    static size_t cache_size = 0;   /* allocated size of cache   */
    static size_t max_computed = 0; /* largest index that has been filled */

    /* Ensure the cache can hold indices up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – unlikely for n ≤ 10000 */
        cache       = tmp;
        cache_size  = needed;
    }

    /* Initialise the first two terms once */
    if (max_computed == 0) {
        cache[0] = (seq_t)0U;
        if (cache_size > 1)
            cache[1] = (seq_t)21U;
        max_computed = (cache_size > 1 ? 1 : 0);   /* 0 if only a(0) requested */
    }

    /* Compute missing terms up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)((seq_t)14 * cache[i-1] - cache[i-2] + (seq_t)18);
    }
    if ((size_t)n > max_computed)
        max_computed = (size_t)n;

    return cache[(size_t)n];
}
