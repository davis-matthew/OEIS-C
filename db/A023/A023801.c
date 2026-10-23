/*
 * A023801  Xenodromes: numbers such that all digits in base 6 are different.
 * https://oeis.org/A023801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = all non‑negative integers whose base‑6 digits are all
 *            different
 * Range:     u8 n <= 143, u16 n <= 1631, u32 n <= 1631, u64 n <= 1631,
 *            u128 n <= 1631, big any n
 * Helpers:   distinct_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (xenodromes).  The sequence is finite; there are exactly 1631 such numbers. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/distinct_base6.h"

seq_t a(idx_t n)
{
    /* static cache of all 1631 xenodromes */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1631;                /* upper bound known from combinatorics */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;                    /* memory error – return 0 as safe fallback */
        size_t count = 0;
        for (seq_t x = 0; count < cache_cap; ++x) {
            if (distinct_base6(x)) {
                cache[count++] = x;
            }
        }
        cache_size = cache_cap;
    }

    /* return nth value (1‑based index) if within bounds */
    if (n >= 1 && (size_t)n <= cache_size)
        return cache[n - 1];
    return 0;    /* undefined for n > cache_size */
}
