/*
 * A169540  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 45 * Σ_{k=1}^{33} a(n-k) - 1035 * a(n-34) + c_n, where
 *            c_n = 1 for n = 0 or n = 34, c_n = 2 for 1 ≤ n ≤ 33, c_n = 0
 *            otherwise
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to return the constant term c_n. */
static inline seq_t c_val(idx_t i)
{
    if (i == 0 || i == 34)
        return (seq_t)1;
    if (i <= 33)
        return (seq_t)2;
    return (seq_t)0;
}

/* Computes a(n). */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of allocated entries */

    /* Grow cache if necessary */
    if ((idx_t)(n + 1) > cached_len) {
        idx_t new_size = n + 1;
        seq_t *tmp;
        if (cached_len == 0) {
            tmp = (seq_t *)malloc(sizeof(seq_t) * new_size);
            if (!tmp) abort();
            for (idx_t i = 0; i < new_size; ++i) tmp[i] = (seq_t)0;
        } else {
            tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
            if (!tmp) abort();
            /* initialise new slots to 0 */
            for (idx_t i = cached_len; i < new_size; ++i) tmp[i] = (seq_t)0;
        }
        cache = tmp;
        cached_len = new_size;
    }

    /* Ensure a(0) is correctly initialised */
    cache[0] = (seq_t)1;

    /* Compute values that are not yet computed */
    for (idx_t i = 1; i <= n; ++i) {
        /* Skip already computed entries (must be non‑zero) */
        if (cache[i] != (seq_t)0) continue;

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 33; ++k) {
            if (i >= k) sum += cache[i - k];
        }

        seq_t term = (seq_t)45 * sum;
        if (i >= 34) term -= (seq_t)1035 * cache[i - 34];
        term += c_val(i);

        cache[i] = term;
    }

    return cache[n];
}
