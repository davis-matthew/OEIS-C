/*
 * A182430  a(n) = n! - A003149(n-1).
 * https://oeis.org/A182430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - sum_{k=0}^{n-1} k! * (n-1-k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(0) = 1 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static caches for factorials and sequence values */
    static seq_t *fact = NULL;      /* factorials 0..fact_len-1 */
    static idx_t fact_len = 0;
    static idx_t fact_cap = 0;

    static seq_t *cache = NULL;     /* a(0)..cache_len-1 */
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    /* ----- ensure factorials up to n are computed ----- */
    if (n >= fact_len) {
        if (fact_cap < n + 1) {
            size_t new_cap = (size_t)(n + 1) * 2ULL;
            seq_t *new_fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
            fact = new_fact;
            fact_cap = (idx_t)new_cap;
        }
        if (fact_len == 0) {
            fact[0] = 1;
            fact_len = 1;
        }
        for (idx_t i = fact_len; i <= n; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_len = n + 1;
    }

    /* ----- compute a(n) and cache all values up to n ----- */
    if (n >= cache_len) {
        if (cache_cap < n + 1) {
            size_t new_cap = (size_t)(n + 1) * 2ULL;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = new_cache;
            cache_cap = (idx_t)new_cap;
        }
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
            } else {
                idx_t m = i - 1;
                idx_t half = m / 2;
                seq_t sum = 0;
                for (idx_t k = 0; k <= half; ++k) {
                    seq_t term = fact[k] * fact[m - k];
                    if (k < m - k) term = term * 2;
                    sum += term;
                }
                cache[i] = fact[i] - sum;
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
