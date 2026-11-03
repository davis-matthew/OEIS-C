/*
 * A097041  Expansion of (1+x)/(1-x^2-9*x^3).
 * https://oeis.org/A097041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 9*a(n-3); a(0) = 1, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 55, u128 n <= 110,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 973 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cached = 0;

/* compute a(n) */
seq_t a(idx_t n)
{
    /* initialise first three terms */
    if (cached == 0) {
        cache = mem_new(seq_t, 3);
        cache[0] = cache[1] = cache[2] = (seq_t)1;
        cached = 3;
    }

    /* extend cache if necessary */
    if (n >= cached) {
        idx_t newsize = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, newsize);
        if (!tmp) return 0;  /* allocation failure, unlikely */
        cache = tmp;
        /* compute missing values */
        for (idx_t i = cached; i <= n; ++i) {
            cache[i] = cache[i-2] + (seq_t)9 * cache[i-3];
        }
        cached = newsize;
    }

    return cache[n];
}
