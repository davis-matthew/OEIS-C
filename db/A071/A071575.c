/*
 * A071575  Number of iterations of cototient(n) needed to reach 1
 *          (cototient(n) = n-phi(n)).
 * https://oeis.org/A071575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n - φ(n)) + 1, a(1) = 0 (cototient iterations)
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/calc_phi.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* make sure the cache can hold indices up to n */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* initialise a(1) */
    cache[1] = 0;

    /* compute values in increasing order */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t phi_i = calc_phi((seq_t)i);      /* φ(i) */
        idx_t j = i - (idx_t)phi_i;            /* i - φ(i) < i */
        cache[i] = cache[j] + 1;               /* recurrence */
    }

    return cache[n];
}
