/*
 * A087579  a(n) = (1/6)*Sum_{k=0..n} binomial(n,k)*Fibonacci(k)*6^k.
 * https://oeis.org/A087579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) + 29*a(n-2); a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache   = NULL; /* array storing a(0) ... a(current_max) */
    static idx_t max_computed = 0;/* last index that has a valid value */
    static idx_t array_size   = 0;/* current allocated length of cache */

    /* Grow cache if necessary */
    if (n + 1 > array_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failure: return 0 as a fallback */
        cache = tmp;
        array_size = new_size;
    }

    /* Initialize the base values whenever the cache is large enough */
    if (array_size >= 1) cache[0] = (seq_t)1;
    if (array_size >= 2) cache[1] = (seq_t)8;

    /* Update the record of how far we have computed */
    if (max_computed < 1) max_computed = (array_size >= 2 ? 1 : 0);

    /* Compute any missing values up to n using the linear recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)8 * cache[i - 1] + (seq_t)29 * cache[i - 2];
    }
    max_computed = n;

    return cache[n];
}
