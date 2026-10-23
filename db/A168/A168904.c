/*
 * A168904  Number of reduced words of length n in Coxeter group on 35
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n > 0 (1 ≤ n ≤ 20): a(n) = 2 + 33 * Σ_{j=1}^{n}
 *            a(n-j) (no a(n-21) term) For n = 21: a(21) = 1 + 33 *
 *            Σ_{j=1}^{20} a(21-j) - 561 * a(0) For n > 21: a(n) = 33 *
 *            Σ_{j=1}^{20} a(n-j) - 561 * a(n-21). (no p_n term) (The first
 *            term obeys p_0 = 1, p_1..p_20 = 2, p_21 = 1, p_n = 0 for
 *            n>21....
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle trivial small values */
    if (n == 0) return (seq_t)1;

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements currently computed */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Ensure enough space for required index */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* out of memory: return 0 as sentinel */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms up to n if not already done */
    for (idx_t k = cache_size; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        /* Sum the previous up to 20 terms */
        for (idx_t j = 1; j <= 20 && j <= k; ++j) {
            sum += cache[k - j];
        }

        seq_t val;
        if (k == 0) {
            val = (seq_t)1;
        } else if (k <= 20) {
            /* p_k = 2 for 1..20 */
            val = (seq_t)2 + (seq_t)33 * sum;
        } else if (k == 21) {
            /* p_21 = 1 */
            val = (seq_t)1 + (seq_t)33 * sum - (seq_t)561 * cache[0];
        } else { /* k > 21 */
            val = (seq_t)33 * sum - (seq_t)561 * cache[k - 21];
        }

        cache[k] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
