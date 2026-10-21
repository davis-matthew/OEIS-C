/*
 * A062181  Harmonic mean of digits is 3.
 * https://oeis.org/A062181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_hm3, resize_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062181: Harmonic mean of digits is 3
 * a(n) = the n-th positive integer whose decimal digits have harmonic mean 3
 * (i.e. for a number with k digits d₁…d_k: k / Σ(1/dᵢ) = 3)
 */

#include <stdlib.h>
#include "../utils/is_hm3.h"
#include "../utils/resize_cache.h"

/* Public sequence function */
seq_t a(idx_t n)
{
    /* undefined for n = 0; return 0 just to keep a valid value */
    if (n == 0) return (seq_t)0;

    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t cur = (seq_t)1;          /* next candidate to test */

    /* Grow cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        if (is_hm3(cur)) {
            cache = resize_cache(cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = cur;
        }
        ++cur;                            /* try next integer */
    }

    return cache[(size_t)n - 1];
}
