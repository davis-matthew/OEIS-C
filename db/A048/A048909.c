/*
 * A048909  9-gonal (or nonagonal) triangular numbers.
 * https://oeis.org/A048909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 255*a(n-1) - 255*a(n-2) + a(n-3) for n >= 4 Initial
 *            values: a(1) = 1 a(2) = 325 a(3) = 82621
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_capacity.h"  /* for ensure_capacity */

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;    /* largest n computed (0 means none yet) */

    if (n == 0) {
        return 0;                    /* offset is 1, return 0 for n=0 */
    }

    /* ensure capacity for indices up to n (1‑based) */
    if (max_computed < n) {
        ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache,
                        sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[i] = 1;
            } else if (i == 2) {
                cache[i] = 325;
            } else if (i == 3) {
                cache[i] = 82621;
            } else {
                /* recurrence a(n) = 255*a(n-1) - 255*a(n-2) + a(n-3) */
                cache[i] = 255 * cache[i - 1] - 255 * cache[i - 2] + cache[i - 3];
            }
        }
        max_computed = n;
    }
    return cache[n];
}
