/*
 * A230179  Number of n X 3 0..2 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          2-x(i,j).
 * https://oeis.org/A230179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 4*a(n-2) Initial values: a(1) = 3, a(2) = 27
 *            (offset = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;      /* undefined for the offset, but returning 0 is harmless */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;   /* cache[0] is unused, indices start at 1 */
    static size_t cap   = 0;      /* allocated size (excluding index 0)   */
    static idx_t last   = 0;      /* highest n already computed */

    /* If the requested index is already in the cache, return it */
    if (n <= last)
        return cache[n];

    /* Ensure the cache can hold indices up to n */
    size_t new_cap = cap ? cap : 1;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    if (cache == NULL) {
        /* allocate new array of size new_cap + 1 to keep index 0 unused */
        cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    }

    /* in case realloc failed, we could handle the error, but for
       this simple implementation we assume enough heap space. */

    cap = new_cap;

    /* Compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)3;
        } else if (i == 2) {
            cache[2] = (seq_t)27;
        } else {
            /* Avoid signed/unsigned mix by casting constants to seq_t */
            cache[i] = cache[i-1] * ((seq_t)9) - cache[i-2] * ((seq_t)4);
        }
    }

    last = n;
    return cache[n];
}
