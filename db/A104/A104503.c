/*
 * A104503  Coefficients of the C-Dyson Mod 27 identity.
 * https://oeis.org/A104503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   coefficients of ∏_{k>0}
 *            (1-x^(27k))(1-x^(27k-6))(1-x^(27k-21))/(1-x^k)
 * Range:     u8 n <= 17, u16 n <= 45, u32 n <= 137, u64 n <= 452,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate DP array for indices 0..n */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) abort();

    /* Initialize */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = (seq_t)0;

    /* Allowed residues modulo 27 (exclude 0, 6, 21) */
    for (idx_t r = 1; r <= 26; ++r) {
        if (r == 6 || r == 21) continue;
        for (idx_t part = r; part <= n; part += 27) {
            for (idx_t j = part; j <= n; ++j) {
                dp[j] += dp[j - part];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
