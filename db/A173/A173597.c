/*
 * A173597  a(n) = a(n-1) + a(n-2) - [a(n-4)/4] - [a(n-5)/2] - [a(n-6)/4].
 * https://oeis.org/A173597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n−1) + a(n−2) − [a(n−4)/4] − [a(n−5)/2] − [a(n−6)/4]
 *            a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 5, a(5) = 8, a(6)
 *            = 13
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values for n = 0 … 6 */
    static const seq_t init[7] = {1, 1, 2, 3, 5, 8, 13};

    if (n < 7)
        return init[(size_t)n];

    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* Allocate and initialise cache if needed */
    if (!cache) {
        cache_cap = n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory – caller must handle */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init[i];
        cache_len = 7;
    }
    /* Grow cache if requested index is beyond current capacity */
    else if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t val = cache[i - 1] + cache[i - 2];
            val -= cache[i - 4] / 4;
            val -= cache[i - 5] / 2;
            val -= cache[i - 6] / 4;
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
