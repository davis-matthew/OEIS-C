/*
 * A101152  Nonnegative values x of solutions (x, y) to the Diophantine
 *          equation x^2+(x+569)^2 = y^2.
 * https://oeis.org/A101152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 29, u64 n <= 65, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-3) - a(n-6) + 1138  for n > 6
 * Initial values: a(1)=0, a(2)=111, a(3)=1260,
 *                 a(4)=1707, a(5)=2280, a(6)=8791
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms; index 0 is unused */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated slots (index 0 unused) */
    static idx_t max_computed = 0; /* largest index for which cache[i] is valid */

    /* Predefined initial values (index 0 unused so we shift by 1) */
    static const seq_t init[7] = {
        0,          /* dummy at index 0 */
        0,          /* a(1) */
        111,        /* a(2) */
        1260,       /* a(3) */
        1707,       /* a(4) */
        2280,       /* a(5) */
        8791        /* a(6) */
    };

    /* Return early for the first six terms */
    if (n <= 6)
        return init[n];

    /* Allocate (or grow) the cache to accommodate index n */
    if (cache_size <= n) {
        idx_t new_size = n + 1; /* need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0; /* allocation failure: undefined output */
        cache = tmp;
        cache_size = new_size;

        /* Ensure indices 1..6 are initialized in the cache */
        if (max_computed < 6) {
            for (idx_t i = 1; i <= 6; ++i)
                cache[i] = init[i];
        }
    }

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 6) {
            cache[i] = init[i];
        } else {
            cache[i] = (seq_t)6 * cache[i - 3] - cache[i - 6] + (seq_t)1138;
        }
    }
    if (n > max_computed)
        max_computed = n;

    return cache[n];
}
