/*
 * A231689  a(n) = Sum_{i=0..n} digsum(i)^4, where digsum(i) = A007953(i).
 * https://oeis.org/A231689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum(i)^4
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 9398, u64 n <= 9398,
 *            u128 n <= 9398, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for the OEIS sequence A231689.
   seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Static cache for accumulated sums */
    static seq_t *cache = NULL;   /* array of cumulative sums */
    static idx_t   cache_max = 0; /* largest n for which cache[n] is valid */
    static size_t  cache_cap = 0; /* allocated length of cache */

    /* If this is the first call, allocate and compute up to n. */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort(); /* allocation failure */

        /* Compute cumulative sums from 0 to n. */
        for (idx_t i = 0; i <= n; ++i) {
            idx_t x = i;
            idx_t sum_digits = 0;
            while (x) {
                sum_digits += x % 10;
                x /= 10;
            }
            seq_t d4 = (seq_t)sum_digits * sum_digits *
                       sum_digits * sum_digits;
            if (i == 0)
                cache[0] = d4;
            else
                cache[i] = cache[i - 1] + d4;
        }
        cache_max = n;
        return cache[n];
    }

    /* If a larger n is requested, grow the cache. */
    if (n > cache_max) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort(); /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute new entries from cache_max+1 up to n. */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            idx_t x = i;
            idx_t sum_digits = 0;
            while (x) {
                sum_digits += x % 10;
                x /= 10;
            }
            seq_t d4 = (seq_t)sum_digits * sum_digits *
                       sum_digits * sum_digits;
            cache[i] = cache[i - 1] + d4;
        }
        cache_max = n;
    }

    /* Return the cached value. */
    return cache[n];
}
