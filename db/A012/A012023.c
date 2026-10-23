/*
 * A012023  Expansion of e.g.f. cos(sin(arctan(x))) (even powers).
 * https://oeis.org/A012023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(12*n^2-24*n+13)*a(n-1) -12*(n-2)*(n-1)*(2*n-3)^2*a(n-2)
 *            -16*(n-3)*(n-2)^2*(n-1)*(2*n-5)*(2*n-3)*a(n-3) a(0)=1, a(1)=-1,
 *            a(2)=13
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   alloc_seq_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed_len = 0;

    /* Ensure the cache can hold the requested index */
    if (n + 1ULL > computed_len) {
        /* Grow the cache if necessary */
        if (cache == NULL) {
            size_t init_cap = 16;
            if (init_cap <= n) init_cap = n + 1;
            cache = alloc_seq_array(init_cap);
            cache_cap = init_cap;
        } else if (n + 1ULL > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < n + 1ULL) new_cap = n + 1ULL;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* Initialize base cases once */
        if (computed_len <= 0ULL) { cache[0] = 1; computed_len = 1; }
        if (computed_len <= 1ULL) { cache[1] = -1; computed_len = 2; }
        if (computed_len <= 2ULL) { cache[2] = 13; computed_len = 3; }

        /* Compute terms up to n using the recurrence */
        for (idx_t i = computed_len; i <= n; ++i) {
            if (i < 3ULL) continue;     /* already set base values */
            seq_t prev1 = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t prev3 = cache[i - 3];

            seq_t two_n   = (seq_t)2 * (seq_t)i;
            seq_t coeff1 = (seq_t)12 * (seq_t)i * (seq_t)i
                           - (seq_t)24 * (seq_t)i
                           + (seq_t)13;
            seq_t coeff2 = (seq_t)12 * (seq_t)(i - 2) * (seq_t)(i - 1)
                           * (two_n - (seq_t)3) * (two_n - (seq_t)3);
            seq_t coeff3 = (seq_t)16 * (seq_t)(i - 3) * (seq_t)(i - 2)
                           * (seq_t)(i - 2) * (seq_t)(i - 1)
                           * (two_n - (seq_t)5) * (two_n - (seq_t)3);

            cache[i] = -coeff1 * prev1 - coeff2 * prev2 - coeff3 * prev3;
        }

        computed_len = n + 1;
    }

    return cache[n];
}
