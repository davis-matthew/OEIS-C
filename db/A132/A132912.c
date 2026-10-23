/*
 * A132912  a(n) = C(n+2,2)*(2*n)!/2^n.
 * https://oeis.org/A132912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n+2,2)*(2*n)!/2^n Recurrence: a(0)=1; a(n) =
 *            (n+2)*(2*n-1)*a(n-1) for n>=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* initialise cache with a(0)=1 if empty */
    if (cache_len == 0) {
        ensure_capacity(1, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* already computed? */
    if (n < cache_len)
        return cache[n];

    /* grow cache to accommodate up to a(n) */
    ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0)
            continue;
        seq_t factor = ((seq_t)i + (seq_t)2) *
                       ((((seq_t)i * (seq_t)2) - (seq_t)1));
        cache[i] = cache[i - 1] * factor;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
