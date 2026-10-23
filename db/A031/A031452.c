/*
 * A031452  Numbers whose base-3 representation has the same number of 0's as
 *          2's.
 * https://oeis.org/A031452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_equal_0_2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A031452: numbers whose base‑3 representation has the same number of 0's and 2's */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/has_equal_0_2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* safety guard for offset */

    static seq_t *cache     = NULL;            /* stored sequence values   */
    static size_t cached_sz = 0;              /* number of cached values  */
    static size_t capacity   = 0;              /* allocated capacity       */
    static seq_t candidate   = 1;              /* next integer to test     */

    /* Generate values until we have at least n of them. */
    while (cached_sz < (size_t)n) {
        if (has_equal_0_2(candidate)) {
            if (cached_sz == capacity) {       /* grow the array           */
                size_t new_cap = capacity ? capacity << 1 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                capacity = new_cap;
            }
            cache[cached_sz++] = candidate;
        }
        /* advance to next integer */
        candidate++;
    }

    return cache[n - 1];
}
