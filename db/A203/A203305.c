/*
 * A203305  Vandermonde determinant of the first n terms of (1,3,7,15,31,...).
 * https://oeis.org/A203305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n-1} Product_{j=1..k} (2^(k+1) - 2^j)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   ensure_cache, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Static cache to reuse computed values. */
    static seq_t *cache = NULL;   /* array a[1]..a[max_calc] */
    static idx_t  max_calc = 0;
    static size_t cache_cap = 0;

    if (n == 0)          /* not part of the sequence */
        return (seq_t)0;

    /* Make sure the cache can hold the requested index. */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the base case a(1)=1 if the cache is empty. */
    if (max_calc == 0) {
        cache[1] = (seq_t)1;
        max_calc = 1;
    }

    /* Compute values up to the requested n using the recurrence:  */
    /* a(k) = a(k-1) * \prod_{j=1}^{k-1} (2^k - 2^j)   for k ≥ 2.     */
    for (idx_t k = max_calc + 1; k <= n; ++k) {
        seq_t pow_k = pow2(k);          /* 2^k */
        seq_t prod  = (seq_t)1;         /* ∏(2^k - 2^j) */

        for (idx_t j = 1; j < k; ++j) {
            seq_t term = pow_k - pow2(j);
            prod *= term;
        }
        cache[k] = cache[k - 1] * prod;
    }
    max_calc = n;
    return cache[n];
}
