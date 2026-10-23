/*
 * A169352  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence a(n) satisfies a linear recurrence of order 30. Let
 *            a(0) = 1. For 1 ≤ n ≤ 30 the recurrence has a constant term +1
 *            on the right‑hand side, and for n ≥ 31 it is homogeneous.
 *            Equivalently: a(n) = ∑_{k=1}^{30} w_k · a(n‑k), where w_k = 2 if
 *            k is odd, –1 if k is even. For n ≤ 30 the same...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n) {
    /* Cache of already computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_cnt = 0;

    /* Coefficients for the recurrence: w_k for k=1..30. */
    static int w[31];
    static int coeff_inited = 0;
    if (!coeff_inited) {
        w[0] = 0;        /* unused */
        for (int k = 1; k <= 30; ++k)
            w[k] = (k & 1) ? 2 : -1;
        coeff_inited = 1;
    }

    /* Expand the cache so that it contains values up to n. */
    if (cache_cnt <= n) {
        idx_t old_cnt = cache_cnt;
        idx_t new_cnt = n + 1;

        if (cache == NULL) {
            cache_cap = new_cnt;
            cache = seq_alloc(cache_cap);
        } else if (new_cnt > cache_cap) {
            cache = seq_realloc(cache, cache_cap, new_cnt);
            cache_cap = new_cnt;
        }

        /* Compute the missing entries. */
        for (idx_t i = old_cnt; i <= n; ++i) {
            seq_t sum = (i <= 30) ? 1 : 0;    /* +1 for n ≤ 30 */
            for (int k = 1; k <= 30; ++k) {
                if (i < (idx_t)k) continue;
                seq_t val = cache[i - k];
                /* w[k] is either 2 or -1 */
                if (w[k] == 2)      sum += (val << 1);
                else                sum -= val;
            }
            cache[i] = sum;
        }
        cache_cnt = new_cnt;
    }

    return cache[n];
}
