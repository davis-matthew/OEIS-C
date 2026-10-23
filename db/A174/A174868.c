/*
 * A174868  Partial sums of Stern's diatomic series A002487.
 * https://oeis.org/A174868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} Stern's diatomic sequence A002487(i).
 *            Recurrence: a(0)=0, a(1)=1; a(2k) = 2*a(k) + a(k-1); a(2k+1) =
 *            2*a(k) + a(k+1);
 * Range:     u8 n <= 49, u16 n <= 1643, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* Cache for computed values.  Values are stored at indices
       0..n after ensure_cache is called.  cache[0] and cache[1]
       are always initialized to 0 and 1 respectively. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static int initialized = 0;

    /* First initialize cache for the first two values. */
    if (!initialized) {
        ensure_cache(2, &cache, &cache_cap);   /* indices 0 and 1 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        initialized = 1;
    }

    /* Ensure we have space for index n. */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Directly return base cases. */
    if (n == 0) return cache[0];
    if (n == 1) return cache[1];

    /* Return cached value if already computed.  Since all
       values for n>1 are positive, a zero entry indicates
       that the value has not yet been computed. */
    if (cache[n] != (seq_t)0) return cache[n];

    /* Compute by the recurrence. */
    seq_t result;
    if ((n & 1) == 0) {                      /* n = 2*k */
        idx_t k = n >> 1;
        seq_t ak   = a(k);
        seq_t akm1 = a(k - 1);
        result = ak * (seq_t)2 + akm1;
    } else {                                 /* n = 2*k + 1 */
        idx_t k = (n - 1) >> 1;
        seq_t ak   = a(k);
        seq_t akp1 = a(k + 1);
        result = ak * (seq_t)2 + akp1;
    }

    cache[n] = result;
    return result;
}
