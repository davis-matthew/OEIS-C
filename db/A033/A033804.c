/*
 * A033804  Product t2(q^d); d | 45, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|45} (∑_{k≥0} q^{d·k(k+1)/2})
 * Range:     u8 n <= 137, u16 n <= 2102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) with dynamic programming over the 6 divisors of 45. */
/* No external libraries are required. */

#include <stdint.h>
#include <stdlib.h>

static const idx_t DIVS[6] = {1, 3, 5, 9, 15, 45};

seq_t a(idx_t n)
{
    /* Allocate and initialize dp[0] = 1 */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure handled as 0 */
    dp[0] = 1;

    for (int di = 0; di < 6; ++di) {
        idx_t d = DIVS[di];
        /* Temporary array for the current divisor */
        seq_t *next = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!next) { free(dp); return 0; }

        for (idx_t sum = 0; sum <= n; ++sum) {
            seq_t cur = dp[sum];
            if (!cur) continue;

            /* Enumerate possible k for this divisor */
            for (idx_t k = 0;; ++k) {
                idx_t tri = k * (k + 1) / 2;     /* k(k+1)/2 */
                idx_t coin = d * tri;            /* d·k(k+1)/2 */
                if (coin > n || sum + coin > n) break;
                next[sum + coin] += cur;
            }
        }

        free(dp);
        dp = next;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
