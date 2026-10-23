/*
 * A046177  Squares (A000290) which are also hexagonal numbers (A000384).
 * https://oeis.org/A046177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 1225, a(3) = 1413721. For n ≥ 4 : a(n) = 1154 *
 *            a(n-1) - a(n-2) + 72. This is the two‑term recurrence equivalent
 *            to a(n+2) = 1154*a(n+1) - a(n) + 72 from OEIS
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is undefined for n = 0 by the problem statement. */
    if (n == 0) return 0;

    /* Convert the 64‑bit index to a size_t for array indexing. */
    size_t idx = (size_t)n;

    /* Static cache for already computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_cap  = 0;   /* allocated size (next index is cache_cap) */
    static size_t cache_last = 0;   /* highest index that has been computed */

    /* Grow the cache if necessary. */
    size_t need = idx + 1;      /* we need to address cache[idx] */
    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) {               /* allocation failed */
            cache = NULL;
            cache_cap  = 0;
            cache_last = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = need;
    }

    /* If the value is already cached, return it. */
    if (idx <= cache_last) {
        return cache[idx];
    }

    /* Compute any missing terms up to idx. */
    for (size_t i = cache_last + 1; i <= idx; ++i) {
        seq_t v;
        if (i == 1) {
            v = (seq_t)1;
        } else if (i == 2) {
            v = (seq_t)1225;
        } else if (i == 3) {
            v = (seq_t)1413721;
        } else {
            /* recurrence a(i) = 1154*a(i-1) - a(i-2) + 72. */
            v = (seq_t)1154 * cache[i - 1] - cache[i - 2] + (seq_t)72;
        }
        cache[i] = v;
    }

    cache_last = idx;
    return cache[idx];
}
