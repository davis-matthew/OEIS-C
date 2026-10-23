/*
 * A198943  y-values in the solution to 10*x^2-9 = y^2.
 * https://oeis.org/A198943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198943:  a(n+6) = 38*a(n+3) – a(n),  a(1)=1, a(2)=9, a(3)=41, a(4)=79, a(5)=351, a(6)=1559 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A zero or negative index is not used; return 0 for safety. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache that grows with the maximum index seen so far. */
    static seq_t *cache = NULL;       /* pointer to array of computed terms */
    static idx_t cache_size = 0;      /* number of elements allocated */
    static idx_t max_filled = 0;      /* number of elements computed */

    /* Ensure the cache is large enough. */
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;
        cache_size = n;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = max_filled; i < n; ++i) {
        if (i <= 5) {                 /* seed the first six values */
            switch (i) {
                case 0: cache[i] = (seq_t)1;     break;
                case 1: cache[i] = (seq_t)9;     break;
                case 2: cache[i] = (seq_t)41;    break;
                case 3: cache[i] = (seq_t)79;    break;
                case 4: cache[i] = (seq_t)351;   break;
                case 5: cache[i] = (seq_t)1559;  break;
            }
        } else {
            /* a(n) = 38*a(n-3) – a(n-6)   (1‑based indices) */
            cache[i] = (seq_t)38 * cache[i - 3] - cache[i - 6];
        }
    }
    max_filled = n;

    /* Return the requested term (array is zero‑based). */
    return cache[n - 1];
}
