/*
 * A037706  Base 8 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,3,0,2.
 * https://oeis.org/A037706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8 * a(n-1) + d[(n-1) mod 4], a(1) = 1 where d = {1,3,0,2}
 *            is the period
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0] holds a(1) */
static idx_t cache_cap = 0;   /* number of terms cached */

seq_t a(idx_t n)
{
    /* Behavior for n < 1 is not defined in OEIS. Return 0. */
    if (n < 1) return 0;

    /* Expand cache if necessary */
    if (n > cache_cap) {
        size_t new_cap = (size_t)n;
        seq_t *new_cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;   /* allocation failure */
        cache = new_cache;

        /* Compute missing terms starting from cache_cap up to n-1 (0‑based index) */
        for (idx_t i = cache_cap; i < n; ++i) {
            /* previous term (i==0 -> a(1) is computed from 0) */
            seq_t prev = (i == 0) ? 0 : cache[i - 1];

            /* digit from the period: 0‑based position i */
            unsigned digit;
            switch (i & 3) {          /* equivalent to i mod 4 */
                case 0: digit = 1; break;
                case 1: digit = 3; break;
                case 2: digit = 0; break;
                case 3: digit = 2; break;
                default: digit = 0; /* unreachable */
            }

            cache[i] = prev * 8 + digit;
        }

        cache_cap = n;
    }

    return cache[n - 1];
}
