/*
 * A077291  Second member of Diophantine pair (m,k) that satisfies 6*(m^2 + m)
 *          = k^2 + k: a(n) = k.
 * https://oeis.org/A077291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-2) - a(n-4) + 4 for n ≥ 4 Base values: a(0)=0,
 *            a(1)=3, a(2)=8, a(3)=35
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 996 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;

    /* initial data for n = 0 … 3 */
    if (cache == NULL) {
        cache_sz = 4;                     /* contains indices 0–3 */
        cache = mem_new(seq_t, cache_sz);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;
        cache[3] = (seq_t)35;
    }

    /* if the requested index is already cached, return it */
    if (n < cache_sz)
        return cache[n];

    /* need to extend the cache up to index n */
    /* old size before growing */
    idx_t old_sz = cache_sz;
    cache_sz = n + 1;
    cache = mem_resize(seq_t, cache, cache_sz);

    /* compute new entries only; indices 0‑3 already set */
    for (idx_t i = old_sz; i <= n; ++i) {
        /* recurrence only valid for i >= 4; for i < 4 the
           initial values are already present */
        cache[i] = (seq_t)(10 * cache[i - 2] - cache[i - 4] + 4);
    }

    return cache[n];
}
