/*
 * A087860  Expansion of e.g.f.: (1-exp(x/(x-1)))/(1-x).
 * https://oeis.org/A087860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = n! * (1 - LaguerreL(n, 1)) * Recurrence (for n ≥ 3): *
 *            a(0) = 0, a(1) = 1, a(2) = 3 * a(n) = 3*(n-1)*a(n-1) -
 *            (n-1)*(3*n-5)*a(n-2) + (n-2)^2*(n-1)*a(n-3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t max_computed = 2;  /* index of last computed term */

    /* ensure cache has at least indices 0..2 defined */
    if (cache_len == 0) {
        cache_len = (n + 1 > 3) ? (n + 1) : 3;
        cache = seq_alloc(cache_len);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 3;
        max_computed = 2;
    } else if (cache_len <= n) {            /* enlarge cache if needed */
        idx_t new_len = n + 1;
        seq_t *tmp = seq_realloc(cache, cache_len, new_len);
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;
        cache_len = new_len;
    }

    /* if already computed, return immediately */
    if (n <= max_computed)
        return cache[n];

    /* compute missing terms */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term1 = cache[i - 1] * ((seq_t)3 * (seq_t)(i - 1));
        seq_t term2 = cache[i - 2] * ((seq_t)(i - 1) *
                                      (seq_t)(3 * i - 5));
        seq_t mult3 = (seq_t)(i - 2) * (seq_t)(i - 2) *
                      (seq_t)(i - 1);
        seq_t term3 = cache[i - 3] * mult3;
        cache[i] = term1 - term2 + term3;
    }
    max_computed = n;
    return cache[n];
}
