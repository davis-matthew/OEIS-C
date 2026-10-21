/*
 * A218348  Number of n X 2 binary arrays with every 0 a horizontal or
 *          vertical neighbor to some 1.
 * https://oeis.org/A218348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:   a(n) = 3*a(n-1) + 2*a(n-2) + 2*a(n-3) - a(n-4) - a(n-5)
   Offset:     1
   Initial terms: a(1)=3, a(2)=11, a(3)=41, a(4)=149, a(5)=547
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for n<1 */

    static seq_t *cache = NULL;          /* 1‑based cache; cache[0] unused */
    static idx_t cache_size = 0;         /* largest n already stored */

    /* if already computed, return immediately */
    if (n <= cache_size) return cache[n];

    /* grow (or create) the cache to hold indices 0..n */
    idx_t new_size = n + 1;               /* number of elements needed */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;                  /* allocation failure */
    cache = tmp;

    /* Base values for indices 1..5 */
    static const seq_t base_vals[6] = {
        (seq_t)0, /* unused, index 0 */
        (seq_t)3,
        (seq_t)11,
        (seq_t)41,
        (seq_t)149,
        (seq_t)547
    };

    /* Ensure that any base indices > cache_size and ≤ n are filled */
    for (idx_t i = 1; i <= 5 && i <= n; ++i) {
        if (i > cache_size) cache[i] = base_vals[i];
    }

    /* Compute recurrence from the next uncomputed index */
    idx_t start = cache_size + 1;
    if (start < 6) start = 6;            /* recurrence requires indices ≥6 */
    for (idx_t i = start; i <= n; ++i) {
        seq_t val = (seq_t)3 * cache[i-1]
                  + (seq_t)2 * cache[i-2]
                  + (seq_t)2 * cache[i-3]
                  - cache[i-4]
                  - cache[i-5];
        cache[i] = val;
    }

    cache_size = n;
    return cache[n];
}
