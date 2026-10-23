/*
 * A178590  a(2n) = 3*a(n), a(2n+1) = a(n) + a(n+1).
 * https://oeis.org/A178590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1; a(2n) = 3 * a(n); a(2n+1) = a(n) + a(n+1); for all n
 *            >= 1 (indexing starts at 1)
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;          /* array of seq_t, 1-based indexing */
    static size_t cache_cap = 0;          /* current allocated size (>= last_filled) */
    static idx_t last_filled = 0;         /* highest index for which cache[i] is valid */

    if (n == 0) return 0;       /* undefined for n==0, but safe return */

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* initialize first term if not already */
    if (last_filled == 0) {
        cache[1] = 1;           /* a(1) = 1 */
        last_filled = 1;
    }

    /* compute new values up to n */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        if ((i & 1) == 0) {     /* even i = 2 * k */
            cache[i] = 3 * cache[i >> 1];
        } else {                /* odd i = 2 * k + 1 */
            idx_t k = i >> 1;   /* k = (i-1)/2 */
            cache[i] = cache[k] + cache[k + 1];
        }
    }
    last_filled = n;
    return cache[n];
}
