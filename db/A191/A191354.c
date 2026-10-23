/*
 * A191354  Number of lattice paths from (0,0) to (n,n) using steps (1,0),
 *          (1,1), (1,2), and (2,1).
 * https://oeis.org/A191354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  n*a(n) = (2*n-1)*a(n-1) + 3*(n-1)*a(n-2) + 2*(2*n-3)*a(n-3)
   Initial values: a(0)=1, a(1)=1, a(2)=3 */
#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;
    static size_t cache_cap = 0;

    /* Ensure cache has space for indices 0..2 */
    if (cache == NULL) {
        ensure_cache(2, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache_max   = 2;
    }

    if (n <= cache_max)
        return cache[n];

    /* Ensure the cache is large enough for the requested index */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
#if defined(__cplusplus)
        /* In the C++ build seq_t is an arbitrary–precision class;
           integer arithmetic is safe here. */
        seq_t term1 = ((seq_t)(2 * i - 1)) * cache[i - 1];
        seq_t term2 = ((seq_t)(3 * (i - 1))) * cache[i - 2];
        seq_t term3 = ((seq_t)(2 * (2 * i - 3))) * cache[i - 3];
        seq_t num   = term1 + term2 + term3;
        cache[i]    = num / (seq_t)i;
#else
        /* In the C build use 128‑bit arithmetic to avoid overflow. */
        unsigned __int128 num = 0;
        num += (unsigned __int128)(2 * i - 1) * (unsigned __int128)cache[i - 1];
        num += (unsigned __int128)(3 * (i - 1)) * (unsigned __int128)cache[i - 2];
        num += (unsigned __int128)(2 * (2 * i - 3)) * (unsigned __int128)cache[i - 3];
        cache[i] = (seq_t)(num / (unsigned __int128)i);
#endif
    }

    cache_max = n;
    return cache[n];
}
