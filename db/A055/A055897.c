/*
 * A055897  a(n) = n*(n-1)^(n-1).
 * https://oeis.org/A055897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)^(n-1) (with 0^0 treated as 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/ensure_cache.h"

/* Exponentiation by squaring for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0)
        return 0;                 /* undefined for n=0 – return 0 */

    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed */
    if (cache[n] != 0)
        return cache[n];

    seq_t base  = (seq_t)(n - 1);
    seq_t power = pow_seq_t(base, n - 1);
    seq_t result = (seq_t)n * power;

    cache[n] = result;             /* cache the computed term */
    return result;
}
