/*
 * A057091  Scaled Chebyshev U-polynomials evaluated at i*sqrt(2). Generalized
 *          Fibonacci sequence.
 * https://oeis.org/A057091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(a(n-1)+a(n-2)), with a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* enlarge cache if necessary */
    if (n + 1 > cache_size) {
        idx_t old = cache_size;
        cache_size = n + 1;
        seq_t *tmp = (seq_t *)realloc((void *)cache,
                                      (size_t)cache_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return 0 as a fallback */
            return (seq_t)0;
        }
        cache = tmp;

        for (idx_t i = old; i < cache_size; ++i) {
            size_t idx = (size_t)i;
            if (i == 0) {
                cache[idx] = (seq_t)1;
            } else if (i == 1) {
                cache[idx] = (seq_t)8;
            } else {
                seq_t sum = cache[idx - 1] + cache[idx - 2];
                cache[idx] = (seq_t)8 * sum;
            }
        }
    }
    return cache[(size_t)n];
}
