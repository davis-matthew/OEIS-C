/*
 * A039057  Numbers whose base-8 representation has the same number of 1's and
 *          3's.
 * https://oeis.org/A039057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = numbers whose base-8 representation has equal number of
 *            1's and 3's
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_balanced_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_balanced_base8.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_count = 0;
    static seq_t last_checked = 0;   /* next candidate to test */

    if (n == 0) {
        return (seq_t)0; /* undefined for offset 1, but safe to return 0 */
    }

    while (cache_count < n) {
        if (is_balanced_base8(last_checked)) {
            /* Ensure cache has space. */
            if (cache_count == cache_cap) {
                idx_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_count++] = last_checked;
        }
        last_checked++;  /* move to next integer */
    }

    return cache[n - 1];
}
