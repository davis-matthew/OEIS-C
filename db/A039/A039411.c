/*
 * A039411  Numbers whose base-10 representation has the same nonzero number
 *          of 2's and 4's.
 * https://oeis.org/A039411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms by scanning integers and selecting those with equal nonzero
   counts of digits 2 and 4.  The sequence values are stored in cached order
   to allow arbitrary call order. */

#include <stdlib.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;    /* number of terms stored */
    static idx_t next_scan = 0;    /* next integer to examine  (starting from 0) */

    if (n == 0)             /* OEIS offset is 1; undefined for n=0 */
        return 0;

    /* Ensure we have computed at least n terms */
    while (cache_len < n) {
        next_scan++;
        unsigned char cnt[10];
        digit_counts((seq_t)next_scan, cnt);

        if (cnt[2] == cnt[4] && cnt[2] > 0) {
            /* Grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                if (new_cap < cache_len + 1)
                    new_cap = cache_len + 1;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* Allocation failed: abort gracefully */
                    free(cache);
                    cache = NULL;
                    cache_cap = cache_len = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_scan;
        }
    }

    return cache[n - 1];
}
