/*
 * A088716  G.f. satisfies: A(x) = 1 + x*A(x)*d/dx[x*A(x)] = 1 + x*A(x)^2 +
 *          x^2*A(x)*A'(x).
 * https://oeis.org/A088716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k * a(k-1) * a(n-k), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;     /* sequence values */
static size_t cache_cap = 0;    /* allocated capacity */
static idx_t  computed_up_to = 0; /* highest index already computed */

seq_t a(idx_t n)
{
    /* make sure the cache is large enough to hold index n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialize a(0) if this is the first call */
    if (computed_up_to == 0 && n >= 0) {
        cache[0] = 1;
        computed_up_to = 0;
    }

    /* compute missing values up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = cache[k - 1];
            term = term * cache[i - k];          /* term = a(k-1) * a(i-k) */
            term = term * (seq_t)k;              /* term *= k */
            sum += term;
        }
        cache[i] = sum;
    }
    computed_up_to = n;

    return cache[n];
}
