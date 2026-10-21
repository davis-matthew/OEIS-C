/*
 * A208088  Number of 7 X (n+1) 0..1 arrays with every 2 X 2 subblock having
 *          the same number of equal edges as its horizontal neighbors and a
 *          different number from its vertical neighbors, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A208088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 37, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A208088: number of 7x(n+1) binary arrays with a
 * Fibonacci-like recurrence
 *
 * a(1) = 108
 * a(2) = 180
 * a(n) = a(n-1) + a(n-2)  for n >= 3
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t
a(idx_t n)
{
    /* The cache stores values for indices 1..size.
     * cache[0] is unused to keep 1-based indexing. */
    static seq_t *cache = NULL;  /* array of seq_t values */
    static idx_t size   = 0;     /* highest index stored in cache */

    /* If the requested index is beyond the cached range,
     * extend the cache and compute the missing values. */
    if (n > size) {
        /* Allocate space for indices 0..n (size n+1). */
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) {          /* allocation failed – return 0 (undefined) */
            return (seq_t)0;
        }
        cache = new_cache;
        /* Compute new entries. */
        for (idx_t i = size + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)108;
            else if (i == 2)
                cache[i] = (seq_t)180;
            else
                cache[i] = cache[i - 1] + cache[i - 2];
        }
        size = n;
    }

    return cache[n];
}
