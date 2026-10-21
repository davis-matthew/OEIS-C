/*
 * A239367  The bisection of A238315 that remains constant with changes in the
 *          offset of the exponent of the second term.
 * https://oeis.org/A239367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-1) - 8*a(n-2) - 2*a(n-3) + 9*a(n-4) - 4*a(n-5)
 * with initial terms:
 *   a(1) = 1, a(2) = 5, a(3) = 13, a(4) = 29, a(5) = 69
 * The sequence is indexed from 1. For n < 1 the function returns 0.
 *
 * The implementation memoises computed values to allow any call order.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices before the offset; defined behaviour is not needed */
    if (n < 1)
        return 0;

    /* Static storage for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated size (elements, index 0 unused) */
    static idx_t last_computed = 0; /* largest n for which cache[n] is valid */

    /* First call: allocate minimum required space and initialise first 5 terms */
    if (cache == NULL) {
        cache_cap = (n > 5) ? n + 1 : 6;          /* at least indices 1..5 */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL)
            abort();                          /* allocation failure */

        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)29;
        cache[5] = (seq_t)69;
        last_computed = 5;                     /* we have values up to a(5) */
    }

    /* Resize cache if needed for the requested n */
    if (n + 1 > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            abort();                          /* allocation failure */
        cache = tmp;
        cache_cap = n + 1;
    }

    /* Compute missing terms up to n, if any */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t v = (seq_t)6 * cache[i - 1]
                    - (seq_t)8 * cache[i - 2]
                    - (seq_t)2 * cache[i - 3]
                    + (seq_t)9 * cache[i - 4]
                    - (seq_t)4 * cache[i - 5];
            cache[i] = v;
        }
        last_computed = n;
    }

    return cache[n];
}
