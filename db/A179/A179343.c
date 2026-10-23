/*
 * A179343  Number of permutations of 1..n with i-6<=p(i)<=i+5.
 * https://oeis.org/A179343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count permutations p of 1..n with i-6 <= p(i) <= i+5.
   The algorithm uses a state‑mask DP with a 12‑column sliding window.
   Complexity: O(n * 2^12 * 12). */

#include <stdlib.h>
#include <stdbool.h>

#define WIDTH (12)
#define MASK_MAX ((1u << WIDTH) - 1)

/* helper: set all elements of an array to zero */
static void zero_seq(seq_t *arr, size_t sz) {
    for (size_t i = 0; i < sz; ++i)
        arr[i] = (seq_t)0;
}

/* main function required by the OEIS build system */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* undefined, return a neutral value */

    const size_t lim = MASK_MAX + 1;

    /* static storage for dynamic arrays, allocated on first use */
    static seq_t *dp = NULL;
    static seq_t *ndp = NULL;
    static size_t dp_sz = 0;

    if (dp_sz < lim) {
        if (dp) {
            free(dp);
            free(ndp);
        }
        dp_sz = lim;
        dp  = (seq_t *)malloc(dp_sz * sizeof(seq_t));
        ndp = (seq_t *)malloc(dp_sz * sizeof(seq_t));
    }

    zero_seq(dp, dp_sz);

    /* Initial state: columns j <= 0 are treated as already used.
       For i=0, the window W0 = [‑5, 6] has six invalid columns
       (–5 … 0).  Set the first six bits to 1. */
    dp[(1u << 6) - 1] = (seq_t)1;   /* bits 0‑5 = 1, bits 6‑11 = 0 */

    for (idx_t i = 1; i <= n; ++i) {
        zero_seq(ndp, dp_sz);

        /* Determine the valid column indices for this row:
           allowed j satisfy max(1, i‑6) ≤ j ≤ min(n, i+5). */
        idx_t lowj  = (i > 6) ? (i - 6) : 1;
        idx_t highj = (i + 5 < n) ? (i + 5) : n;

        /* Compute the corresponding low/high bit positions
           within the current window W_{i‑1} = [i‑6, i+5]. */
        idx_t lowk  = lowj  - (i - 6);
        idx_t highk = highj - (i - 6);

        for (unsigned int mask = 0; mask <= MASK_MAX; ++mask) {
            seq_t cur = dp[mask];
            if (cur == (seq_t)0) continue;

            for (idx_t k = lowk; k <= highk; ++k) {
                /* If this column is already used, skip. */
                if (mask & (1u << k)) continue;

                unsigned int newmask = ((mask | (1u << k)) >> 1);
                ndp[newmask] += cur;
            }
        }

        /* Swap dp and ndp for the next iteration */
        seq_t *tmp = dp;
        dp = ndp;
        ndp = tmp;
    }

    /* After the last row n, we are in window W_n = [n‑5, n+6].
       All valid columns (1..n) must be used. */
    unsigned int required = 0;
    for (idx_t k = 0; k < WIDTH; ++k) {
        idx_t j = (n - 5 + k);          /* column represented by bit k */
        if (j >= 1 && j <= n)
            required |= (1u << k);
    }

    seq_t ans = (seq_t)0;
    for (unsigned int mask = 0; mask <= MASK_MAX; ++mask) {
        if ((mask & required) == required)
            ans += dp[mask];
    }
    return ans;
}
