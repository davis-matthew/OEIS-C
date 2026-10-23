/*
 * A028225  Expansion of 1/((1-7x)(1-9x)(1-10x)(1-11x)).
 * https://oeis.org/A028225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*11^(n+3) - 8*10^(n+3) + 6*9^(n+3) - 7^(n+3)) / 24 * *
 *            The sequence satisfies the linear recurrence * a(0)=1, a(1)=37,
 *            a(2)=860, a(3)=16070 * a(n) = 37*a(n-1) - 509*a(n-2) +
 *            3083*a(n-3) - 6930*a(n-4) (n>=4) * * The implementation caches
 *            computed terms so that any order of calls * works e...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Ensure the cache is large enough. */
    size_t needed = (size_t)n + 1;          /* terms 0..n inclusive */
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failed */
        cache = tmp;

        /* Compute all newly missing terms. */
        for (size_t i = cache_cap; i < needed; ++i) {
            seq_t val;
            if (i == 0)                  val = 1;
            else if (i == 1)             val = 37;
            else if (i == 2)             val = 860;
            else if (i == 3)             val = 16070;
            else {
                val = cache[i-1] * 37;
                val -= cache[i-2] * 509;
                val += cache[i-3] * 3083;
                val -= cache[i-4] * 6930;
            }
            cache[i] = val;
        }
        cache_cap = needed;
    }
    return cache[(size_t)n];
}
