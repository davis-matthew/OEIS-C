/*
 * A169464  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169464: Number of reduced words of length n in a Coxeter group on 19 generators
 * with relations (S_i)^2 = (S_i S_j)^3 = I.
 *
 * Recurrence derived from the generating function:
 *   a(0) = 1
 *   For 1 ≤ n ≤ 32:  a(n) = 2 + 17 * Σ_{k=0}^{n-1} a(k)
 *   a(33) = 1 + 17 * Σ_{k=1}^{32} a(k) - 153 * a(0)
 *   For n ≥ 34:    a(n) = 17 * Σ_{k=1}^{32} a(n-k) - 153 * a(n-33)
 *
 * The sequence is memoised; new terms are computed only once.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache state */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;    /* number of terms stored  */
    static size_t cache_cap  = 0;    /* array capacity          */
    static seq_t total_sum   = 0;    /* sum of all stored terms */
    static seq_t sum32       = 0;    /* sum of the last 32 terms */

    /* Ensure cache is large enough */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = required * 2;
        seq_t *new_cache = seq_alloc_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        if (cache != NULL)
            seq_free_array(cache);
        cache      = new_cache;
        cache_cap  = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val;

        if (i == 0) {
            val = 1;
        } else if (i <= 32) {
            val = 2 + ((seq_t)17) * total_sum;
        } else if (i == 33) {
            seq_t sum_before = total_sum - cache[0];   /* a(1)+…+a(32) */
            val = 1 + ((seq_t)17) * sum_before
                  - ((seq_t)153) * cache[0];
        } else { /* i >= 34 */
            val = ((seq_t)17) * sum32    /* a(i-1)+…+a(i-32) */
                  - ((seq_t)153) * cache[i - 33]; /* a(i-33)   */
        }

        cache[i] = val;
        total_sum += val;

        /* Update sum32 (last 32 terms) */
        sum32 += val;
        if (i >= 32)
            sum32 -= cache[i - 32];
    }

    cache_len = required;
    return cache[(size_t)n];
}
