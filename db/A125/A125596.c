/*
 * A125596  a(1)=1. a(n) = Sum_{1<=k<n, gcd(k,n(n+1))=1} a(k).
 * https://oeis.org/A125596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = Sum_{1 <= k < n, gcd(k, n(n+1)) = 1} a(k) (n >=
 *            2)
 * Range:     u8 n <= 24, u16 n <= 45, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Return a(n) for the OEIS sequence A125596. */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;          /* cache[1..max_cached] */
    static idx_t max_cached = 0;         /* largest n for which cache[n] is valid */

    /* Allocate initial cache if necessary */
    if (!cache) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));   /* allocate space for indices 0 and 1 */
        if (!cache) return 0;                       /* allocation failure: return 0 as safe fallback */
        cache[1] = (seq_t)1;                        /* a(1) = 1 */
        max_cached = 1;
    }

    /* If the requested n is already cached, simply return it */
    if (n <= max_cached) {
        return cache[n];
    }

    /* Grow the cache to accommodate n */
    idx_t new_size = n + 1;                         /* indices 0..n inclusive */
    seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
    if (!new_cache) return 0;                       /* allocation failure */
    /* copy previous values */
    for (idx_t i = 1; i <= max_cached; ++i) {
        new_cache[i] = cache[i];
    }
    free(cache);
    cache = new_cache;

    /* Compute a(m) for all m in (max_cached, n] */
    for (idx_t m = max_cached + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k < m; ++k) {
            /* Check coprimality with m and with m+1 separately.
               Since gcd(k, m) == 1 and gcd(k, m+1) == 1 iff
               gcd(k, m(m+1)) == 1. Both tests are cheap. */
            if (gcd((seq_t)k, (seq_t)m) == 1 &&
                gcd((seq_t)k, (seq_t)(m + 1)) == 1) {
                sum += cache[(size_t)k];
            }
        }
        cache[(size_t)m] = sum;
    }
    max_cached = n;
    return cache[n];
}
