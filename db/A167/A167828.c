/*
 * A167828  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 37 * Σ_{k=1}^{14}
 *            a(n-k) – 703 * a(n-15) + b(n) where b(0) = 1, b(1) … b(14) = 2,
 *            b(15) = 1, b(n) = 0 for n > 15. The initial values therefore
 *            follow from this recurrence with a(j)=0 for j < 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required standard headers */
#include <stddef.h>
#include <stdlib.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored */
    static size_t cache_cap = 0;   /* allocated capacity (bytes) */

    /* Convert n to size_t for array indexing (safe for the limits we handle) */
    size_t idx = (size_t)n;

    /* Ensure the cache is large enough to hold term n */
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;          /* capacity in terms */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* If the requested term is already in the cache, return it */
    if (idx < cache_len)
        return cache[idx];

    /* Compute missing terms up to idx inclusive, extending the cache */
    for (size_t i = cache_len; i <= idx; ++i) {
        seq_t sum = 0;

        /* Σ_{k=1}^{14} a(i-k) weighted by 37 */
        for (size_t k = 1; k <= 14; ++k) {
            if (i >= k)
                sum += cache[i - k] * (seq_t)37;
        }

        /* – 703 * a(i-15) if i ≥ 15 */
        if (i >= 15)
            sum -= cache[i - 15] * (seq_t)703;

        /* Add the b(i) term for i ≤ 15 */
        if (i == 0) {
            sum += (seq_t)1;
        } else if (i <= 14) {
            sum += (seq_t)2;
        } else if (i == 15) {
            sum += (seq_t)1;
        }

        cache[i] = sum;
    }

    cache_len = idx + 1;
    return cache[idx];
}
