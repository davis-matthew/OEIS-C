/*
 * A020919  Partition numbers mod 11.
 * https://oeis.org/A020919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000041(n) mod 11
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Return cached value if available */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow cache to hold up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();            /* fatal error: memory allocation failed */
    cache = new_cache;

    /* Initialise p(0) if cache was just created */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;           /* p(0) = 1 */
        cache_size = 1;
    }

    /* Compute partition numbers mod 11 for all m up to n */
    for (idx_t m = cache_size; m <= n; ++m) {
        seq_t s = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t p1 = k * ((idx_t)3 * k - 1) / 2;
            if (p1 > m) break;
            seq_t t = cache[m - p1];
            if (k & 1) {                    /* odd k → +term */
                s += t;
                if (s >= 11) s -= 11;
            } else {                        /* even k → -term */
                s += (seq_t)11 - t;
                if (s >= 11) s -= 11;
            }
            idx_t p2 = k * ((idx_t)3 * k + 1) / 2;
            if (p2 > m) continue;
            seq_t t2 = cache[m - p2];
            if (k & 1) {
                s += t2;
                if (s >= 11) s -= 11;
            } else {
                s += (seq_t)11 - t2;
                if (s >= 11) s -= 11;
            }
        }
        cache[m] = s;
    }

    cache_size = n + 1;
    return cache[n];
}
