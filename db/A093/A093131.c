/*
 * A093131  Binomial transform of Fibonacci(2n).
 * https://oeis.org/A093131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1; a(n)=5*a(n-1)-5*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoisation table for already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache holds terms up to n */
    if (n + 1 > cache_len) {
        /* Grow cache by realloc, also initialise new slots lazily */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure – return 0 as fallback */
        cache = new_cache;

        /* Initialize first term if we just created the table */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;
            cache_len = 1;
        }

        /* If we need a[1] that hasn't been set yet, initialise it */
        if (cache_len == 1 && n >= 1) {
            cache[1] = (seq_t)1;
            cache_len = 2;
        }

        /* Fill in missing terms using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* a[i] = 5*a[i-1] - 5*a[i-2] */
            seq_t t1 = 5 * cache[i - 1];
            seq_t t2 = 5 * cache[i - 2];
            cache[i] = t1 - t2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
