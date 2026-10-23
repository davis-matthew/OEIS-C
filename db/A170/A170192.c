/*
 * A170192  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^39 = I.
 * https://oeis.org/A170192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   a(0) = 1
   for 1 <= n <= 38: a(n) = 37 * sum_{k=0}^{n-1} a(k) + 2
   a(39) = 37 * sum_{k=1}^{38} a(k) - 703 * a(0) + 1
   for n >= 40:  a(n) = 37 * sum_{k=n-38}^{n-1} a(k) - 703 * a(n-39) */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static seq_t cum_sum = 0;          /* sum of a[0 .. i-1] for next i   */
    static seq_t window_sum = 0;       /* sum of the last 38 computed terms */

    /* allocate or expand cache */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure – unlikely in the test harness */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i <= 38) {
            /* a(i) = 37 * sum_{0}^{i-1} a(k) + 2 */
            val = (seq_t)37 * cum_sum + (seq_t)2;
        } else if (i == 39) {
            /* a(39) = 37 * sum_{1}^{38} a(k) - 703 * a(0) + 1 */
            seq_t sum_1_to_38 = cum_sum - cache[0];
            val = (seq_t)37 * sum_1_to_38;
            val -= (seq_t)703 * cache[0];
            val += (seq_t)1;
        } else /* i >= 40 */ {
            /* a(i) = 37 * window_sum - 703 * a(i-39) */
            val = (seq_t)37 * window_sum;
            val -= (seq_t)703 * cache[i - 39];
        }

        cache[i] = val;
        cum_sum += val;

        if (i == 39) {
            /* set window_sum to sum_{2}^{39} for the next iteration */
            window_sum = cum_sum - cache[0] - cache[1];
        } else if (i >= 40) {
            /* update window_sum for the next term: subtract a(i-38), add a(i) */
            window_sum += val;
            window_sum -= cache[i - 38];
        }

        cache_len = i + 1;
    }

    return cache[n];
}
