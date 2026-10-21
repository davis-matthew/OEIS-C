/*
 * A081073  Fibonacci(4n+2)+3, or Fibonacci(2n-1)*Lucas(2n+3).
 * https://oeis.org/A081073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) + a(n-3) Initial values: a(0) = 4,
 *            a(1) = 11, a(2) = 58
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_len = 0;   /* number of terms stored (indices 0 .. cache_len-1) */

    /* Ensure cache large enough for index n */
    if (n >= cache_cap) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 16;
        while (new_cap <= n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Handle allocation failure – behaviour is undefined for this sequence */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)4;
        } else if (i == 1) {
            cache[i] = (seq_t)11;
        } else if (i == 2) {
            cache[i] = (seq_t)58;
        } else {
            seq_t term = cache[i-1] * (seq_t)8;
            term -= cache[i-2] * (seq_t)8;
            term += cache[i-3];
            cache[i] = term;
        }
    }

    /* Update stored length */
    if (n + 1 > cache_len)
        cache_len = n + 1;

    return cache[n];
}
