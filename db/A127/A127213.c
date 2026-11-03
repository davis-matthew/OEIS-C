/*
 * A127213  a(n) = 6^n*Lucas(n), where Lucas = A000204.
 * https://oeis.org/A127213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 36*a(n-2), a(1) = 6, a(2) = 108 (Equivalently
 *            a(n) = 6^n * Lucas(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 994 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Cache for previously computed terms; index 0 is unused.  */
    static seq_t *cache = NULL;
    static idx_t last = 0;      /* highest index computed so far */
    static size_t capacity = 0; /* allocated cache size */

    if (n < 1)
        return 0; /* Undefined for n < 1; return 0 for safety */

    /* Determine how many array entries are needed (index n included). */
    size_t needed = (size_t)n + 1 < 3 ? 3 : (size_t)n + 1;   /* 1-based; cache[2] is set below */

    /* Allocate or grow the cache.  Use C‑style cast for compatibility. */
    if (cache == NULL) {
        cache = mem_new(seq_t, needed);
        capacity = needed;
    } else if (needed > capacity) {
        seq_t *tmp = mem_new(seq_t, needed);
        /* Copy existing values into the new array. */
        for (idx_t i = 1; i <= last; ++i) {
            tmp[i] = cache[i];
        }
        mem_free(cache);
        cache = tmp;
        capacity = needed;
    }

    /* Initialise base values lazily.  They are placed at indices 1 and 2. */
    if (last < 1) {
        cache[1] = (seq_t)6;
        last = 1;
    }
    if (last < 2) {
        cache[2] = (seq_t)108;
        last = 2;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)6 * cache[i - 1] + (seq_t)36 * cache[i - 2];
    }

    last = n;
    return cache[n];
}
