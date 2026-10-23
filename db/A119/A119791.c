/*
 * A119791  a(1) = 1, a(n) = number of divisors of n*a(n-1).
 * https://oeis.org/A119791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = divisor_count(n * a(n-1)) for n > 1
 * Range:     u8 n <= 18, u16 n <= 1331, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/divisor_count_u64.h"

/* Helper: compute the number of divisors of a 64‑bit integer.
   The sequence values are guaranteed to fit in 64 bits for all n up to 10,000
   in the test harness, so we can safely cast to uint64_t. */
static inline seq_t div_count64(seq_t x) {
    return divisor_count_u64((uint64_t)x);
}

seq_t a(idx_t n) {
    /* static memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;  /* highest n for which a(n) is in cache */

    if (n == 0) return 0; /* undefined, but avoid UB */

    /* Ensure capacity for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute values from (last_computed+1) up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        if (k == 1) {
            cache[0] = 1;                /* a(1) = 1 */
        } else {
            seq_t prod = (seq_t)k * cache[k - 2];
            cache[k - 1] = div_count64(prod);  /* a(k) = d(k * a(k-1)) */
        }
    }
    last_computed = n > last_computed ? n : last_computed;

    return cache[n - 1];
}
