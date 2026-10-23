/*
 * A169015  Number of reduced words of length n in Coxeter group on 50
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A169015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * for 1 ≤ n ≤ 22: a(n) = 48·∑_{i=0}^{n‑1} a(i) + 2 *
 *            a(23) = 48·∑_{i=1}^{22} a(i) – 1176·a(0) + 1 * for n ≥ 24: a(n)
 *            = 48·∑_{i=1}^{22} a(n‑i) – 1176·a(n‑23)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;   /* memoised values */
    static size_t seq_cap = 0;    /* current capacity of seq */
    static idx_t  seq_len = 0;    /* number of values stored (seq[0..seq_len-1]) */

    /* expand storage if necessary */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap <= (size_t)n + 1) new_cap *= 2;
        seq = seq_realloc(seq, seq_cap, new_cap);
        seq_cap = new_cap;
    }

    /* initialise first term if not yet done */
    if (seq_len == 0) {
        seq[0] = (seq_t)1;
        seq_len = 1;
    }

    /* if already computed, return immediately */
    if (seq_len > n) {
        return seq[n];
    }

    /* extend sequence up to index n */
    while (seq_len <= n) {
        idx_t i = seq_len;

        /* sum of all previous terms (needed for the small‑n cases) */
        seq_t sum_prev = 0;
        for (idx_t j = 0; j < i; ++j) {
            sum_prev += seq[j];
        }

        if (i <= 22) {                     /* 1 ≤ i ≤ 22 */
            seq[i] = (seq_t)48 * sum_prev + (seq_t)2;
        } else if (i == 23) {              /* i = 23 */
            seq_t sum_1_to_22 = sum_prev - seq[0];
            seq[i] = (seq_t)48 * sum_1_to_22
                   - (seq_t)1176 * seq[0]
                   + (seq_t)1;
        } else {                           /* i ≥ 24 */
            /* sum of the previous 22 terms */
            seq_t sum_last22 = 0;
            for (idx_t j = i - 22; j < i; ++j) {
                sum_last22 += seq[j];
            }
            seq[i] = (seq_t)48 * sum_last22
                   - (seq_t)1176 * seq[i - 23];
        }

        seq_len = i + 1;
    }

    return seq[n];
}
