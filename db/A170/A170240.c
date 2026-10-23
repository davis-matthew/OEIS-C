/*
 * A170240  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * * For n = 0: a(0) = 1 *
 *            For 1 ≤ n ≤ 39: a(n) = 37·(a(n-1)+…+a(n-39)) + 2 * For n ≥ 40:
 *            a(n) = 37·(a(n-1)+…+a(n-39)) – 703·a(n-40) * * This recurrence
 *            follows from the rational generating function * (t^40 +
 *            2∑_{k=1}^{39} t^k + 1) * / (703 t^40 – 37∑_{k=1}^{39...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached sequence values */
    static seq_t *seq = NULL;
    static idx_t seq_len = 0;
    static size_t seq_cap = 0;

    /* Ensure capacity */
    if (seq == NULL) {
        seq_cap = n + 1 > 16 ? n + 1 : 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq) return 0; /* Allocation failed: return 0 */
    } else if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = seq_cap * 2;
        if (new_cap < (size_t)n + 1) new_cap = n + 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) return 0;
        seq_cap = new_cap;
    }

    /* Compute terms up to n if necessary */
    for (idx_t i = seq_len; i <= n; ++i) {
        seq_t sum = 0;
        /* Sum of previous 39 terms */
        for (idx_t k = 1; k <= 39 && i >= k; ++k) {
            sum += seq[i - k];
        }
        seq_t term = 37 * sum;                 /* 37 * sum */
        if (i >= 40) term -= 703 * seq[i - 40];/* – 703 * a(i-40) for i ≥ 40 */
        if (i == 0) term += 1;                 /* a(0) = 1 */
        else if (i <= 39) term += 2;           /* a(n) += 2 for 1≤n≤39 */

        seq[i] = term;
    }
    seq_len = n + 1;

    return seq[n];
}
