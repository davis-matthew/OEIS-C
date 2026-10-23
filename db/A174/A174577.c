/*
 * A174577  Expansion of 1/(1-x^2-x^3+x^7-x^8+x^10).
 * https://oeis.org/A174577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) - a(n-7) + a(n-8) - a(n-10) (for n ≥ 10)
 *            Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=2,
 *            a(6)=2, a(7)=2, a(8)=5, a(9)=3
 * Range:     u8 n <= 26, u16 n <= 51, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[10] = {
    1, 0, 1, 1, 1,
    2, 2, 2, 5, 3
};

seq_t a(idx_t n)
{
    /* Handle the small indices directly */
    if (n < 10) {
        return init[n];
    }

    static seq_t *cache = NULL;
    static idx_t cached_max = 9;  /* last index computed */
    static size_t cap = 0;

    if (cache == NULL) {
        /* Allocate initial storage and copy initial values */
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (idx_t i = 0; i < 10; ++i) {
            cache[i] = init[i];
        }
    }

    if (n > cached_max) {
        /* Grow the cache if necessary */
        if ((size_t)(n + 1) > cap) {
            size_t new_cap = cap;
            while (new_cap <= (size_t)(n + 1)) {
                new_cap *= 2;
            }
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (idx_t i = cached_max + 1; i <= n; ++i) {
            seq_t val = cache[i - 2] + cache[i - 3] + cache[i - 8];
            val = val - cache[i - 7];
            val = val - cache[i - 10];
            cache[i] = val;
        }

        cached_max = n;
    }

    return cache[n];
}
