/*
 * A164544  a(n) = 2*a(n-1) + 7*a(n-2) for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A164544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) for n > 1; a(0) = 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 175 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* Static cache for on‑demand computation */
    static seq_t *cache      = NULL;
    static idx_t  cache_len  = 0;

    /* Initialisation: store first two terms */
    if (!cache) {
        cache_len = 2;
        cache = mem_new(seq_t, cache_len);
        if (!cache) abort();           /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
    }

    /* Grow cache if necessary */
    idx_t needed = n + 1;
    if (needed > cache_len) {
        seq_t *tmp = mem_resize(seq_t, cache, needed);
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_len; i < needed; ++i) {
            cache[i] =
                ((seq_t)2) * cache[i - 1] + ((seq_t)7) * cache[i - 2];
        }
        cache_len = needed;
    }

    return cache[(size_t)n];
}
