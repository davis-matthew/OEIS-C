/*
 * A015196  Sum of Gaussian binomial coefficients for q=10.
 * https://oeis.org/A015196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   ensure_cache, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + (10^(n-1)-1)*a(n-2),  a(0)=1, a(1)=2 */
#include "../utils/ensure_cache.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Make sure we can store indices up to n */
    ensure_cache(n, &cache, &cap);

    /* Initialize first two terms if not already done */
    if (!cache[0]) cache[0] = (seq_t)1;
    if (!cache[1]) cache[1] = (seq_t)2;

    /* Compute missing terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        if (cache[i]) continue;   /* already computed */
        seq_t pow10 = pow_uint(10, i - 1);
        seq_t term1 = cache[i - 1] * 2;
        seq_t term2 = (pow10 - 1) * cache[i - 2];
        cache[i] = term1 + term2;
    }

    return cache[n];
}
