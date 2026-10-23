/*
 * A010797  a(n) = n! * (n+1)! * (n+2)! / (2! * 3!).
 * https://oeis.org/A010797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * (n+1)! * (n+2)! / (2! * 3!)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(1) = 1; a(n) = a(n-1) * n*(n+1)*(n+2) for n>=2 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache results for repeated calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* number of precomputed elements */

    /* Safety for values below the offset (not required for correctness) */
    if (n == 0) return 0;

    /* Return cached value if already known */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* Extend cache to accommodate up to n */
    seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!tmp) return 0;                 /* allocation failure */
    cache = tmp;

    /* Compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = (seq_t)1;
        } else {
            seq_t prod = (seq_t)i * (seq_t)(i + 1) * (seq_t)(i + 2);
            cache[i - 1] = cache[i - 2] * prod;
        }
    }
    cache_size = n;
    return cache[n - 1];
}
