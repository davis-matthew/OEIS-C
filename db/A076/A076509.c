/*
 * A076509  Expansion of 3*x*(1-x)*(1+2*x+6*x^2)/(1-24*x^3).
 * https://oeis.org/A076509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 24*a(n-3) + c(n), where * c(1)=3, c(2)=3, c(3)=12,
 *            c(4)=-18, c(n)=0 for n>4 * The sequence is defined for n ≥ 1
 *            (offset 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but guard for safety */
    /* Ensure enough space for indices 0..n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute terms up to n, reusing any previously computed values */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue; /* already computed */

        seq_t val;
        if (i == 1 || i == 2) {
            val = (seq_t)3;
        } else if (i == 3) {
            val = (seq_t)12;
        } else if (i == 4) {
            /* 24*a(1) - 18 */
            val = (seq_t)24 * cache[1] - (seq_t)18;
        } else {
            val = (seq_t)24 * cache[i - 3];
        }
        cache[i] = val;
    }
    return cache[n];
}
