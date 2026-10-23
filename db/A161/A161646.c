/*
 * A161646  Number of reduced words of length n in the Weyl group A_33.
 * https://oeis.org/A161646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of x^n in ∏_{k=1}^{33} (1 - x^{k+1})/(1 - x)
 *
 * The sequence is computed by iteratively multiplying the polynomial
 * 1 by each factor (1 - x^{k+1})/(1 - x).  For an intermediate polynomial
 * p(x) = ∑_{i=0}^N p_i x^i and a factor (1 - x^{k+1})/(1 - x),
 * the coefficients of the new polynomial q(x) satisfy
 *
 *     q_i = (∑_{j=0}^i p_j) - (i ≥ k+1 ? ∑_{j=0}^{i-k-1} p_j : 0).
 *
 * Two working arrays are used and swapped after each factor.
 * The results are cached so subsequent calls with larger n only recompute
 * from scratch up to the new maximum.  The algorithm runs in O(M·N)
 * time with M = 33 and N the maximum n requested.
 */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *dp_prev = NULL;    /* coefficients of current polynomial */
static seq_t *dp_next = NULL;    /* temporary array for the next step   */
static idx_t  computed_n = 0;    /* highest n for which dp_prev is valid */
static bool    initialized  = false;

/* Helper to grow an array to at least new_n+1 elements */
static void ensure_size(idx_t new_n) {
    size_t needed = (size_t)new_n + 1;
    if (!dp_prev) {
        dp_prev = (seq_t*)calloc(needed, sizeof(seq_t));
        dp_next = (seq_t*)calloc(needed, sizeof(seq_t));
        computed_n = 0;
    } else if (needed > ((size_t)computed_n + 1)) {
        dp_prev = (seq_t*)realloc(dp_prev, needed * sizeof(seq_t));
        dp_next = (seq_t*)realloc(dp_next, needed * sizeof(seq_t));
    }
}

seq_t a(idx_t n) {
    /* First compute up to the requested n if needed */
    if (!initialized || n > computed_n) {
        idx_t new_n = n;
        ensure_size(new_n);

        /* Start with polynomial 1 */
        for (idx_t i = 0; i <= new_n; ++i) dp_prev[i] = 0;
        dp_prev[0] = 1;

        /* Process each factor (1 - x^{k+1})/(1 - x) for k = 1..33 */
        for (idx_t k = 1; k <= 33; ++k) {
            /* Prefix sums of dp_prev up to new_n */
            seq_t cum = 0;
            for (idx_t i = 0; i <= new_n; ++i) {
                cum = cum + dp_prev[i];
                seq_t prefix = cum;

                /* Subtract prefix up to i - k - 1 if applicable */
                if (i >= k + 1) {
                    /* Compute prefix up to i - k - 1 */
                    /* We can reuse cum from previous iteration via an array, but
                       for simplicity recompute by accumulating again since
                       k <= 33 and new_n is modest. */
                    seq_t cum2 = 0;
                    for (idx_t j = 0; j <= i - k - 1; ++j) cum2 = cum2 + dp_prev[j];
                    dp_next[i] = prefix - cum2;
                } else {
                    dp_next[i] = prefix;
                }
            }
            /* Swap dp_prev and dp_next for next iteration */
            seq_t *tmp = dp_prev;
            dp_prev = dp_next;
            dp_next = tmp;
        }
        computed_n = new_n;
        initialized = true;
    }
    return dp_prev[n];
}
