/*
 * A082310  Duplicate of A015565.
 * https://oeis.org/A082310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (8^n + (-1)^{n+1}) / 9 Equivalent recurrence: a(0) = 0
 *            a(n) = 8 * a(n-1) + ((n & 1) ? 1 : -1) for n ≥ 1
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"
/* seq_t and idx_t are defined by the build */

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (cache_len > 0 ⇒ cache[0] .. cache[cache_len-1]) */

    /* Allocate or grow the cache to include term a[n] */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        /* Reallocate (or allocate if first call) */
        cache = mem_resize(seq_t, cache, new_len);
        if (!cache) return 0;   /* allocation failure – undefined behaviour in this build */

        /* Initialize new entries */
        if (cache_len == 0) {          /* first entry a[0] */
            cache[0] = 0;
            cache_len = 1;
            cache[0] = 0;
        }
        for (idx_t i = cache_len; i < new_len; ++i) {
            /* recurrence: a(i) = 8 * a(i-1) + ((i & 1) ? +1 : -1) */
            seq_t term = (seq_t)8 * cache[i - 1] + ((i & 1) ? (seq_t)1 : (seq_t)(-1));
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n];
}
