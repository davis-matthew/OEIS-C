/*
 * A067951  a(0) = 1; a(n) = Sum_{1 <= k <= n and k|n} a(n-k).
 * https://oeis.org/A067951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k|n, 1 <= k <= n} a(n-k), with a(0)=1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 45, u64 n <= 91, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise the cache with the first term */
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        cache[0] = 1;
    }

    /* Extend the cache up to index n if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            /* Enumerate all positive divisors d of i */
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    sum += cache[i - d];
                    idx_t e = i / d;
                    if (e != d) {
                        sum += cache[i - e];
                    }
                }
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
