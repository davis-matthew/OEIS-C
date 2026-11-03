/*
 * A153060  a(0)=4, a(n)=a(n-1)^2 - n for n > 0.
 * https://oeis.org/A153060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=4, a(n)=a(n-1)^2 - n for n>0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 7 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size (in terms) */
    static idx_t upto  = 0;   /* highest index computed */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache = mem_new(seq_t, 1);
        if (!cache) return (seq_t)0; /* OOM → 0 */
        cache[0] = (seq_t)4;
        cap   = 1;
        upto  = 0;
    }

    /* If requested index already computed, return it */
    if (n <= upto) {
        return cache[n];
    }

    /* Resize cache if necessary */
    if ((idx_t)(n + 1) > cap) {
        seq_t *tmp = mem_resize(seq_t, cache, (n + 1));
        if (!tmp) return (seq_t)0; /* OOM → 0 */
        cache = tmp;
        cap   = n + 1;
    }

    /* Compute missing terms sequentially */
    for (idx_t i = upto + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] * cache[i - 1] - (seq_t)i;
    }

    upto = n;
    return cache[n];
}
