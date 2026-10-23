/*
 * A029429  Expansion of 1/((1-x^6)(1-x^9)(1-x^10)(1-x^12)).
 * https://oeis.org/A029429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coeff of x^n in 1/((1-x^6)(1-x^9)(1-x^10)(1-x^12))
 *            Equivalent: number of non‑negative integer solutions of 6a + 9b
 *            + 10c + 12d = n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Compute the nth term of A029429. */
seq_t a(idx_t n)
{
    /* Allocate an array of size n+1 and initialise to zero. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t*)calloc(len, sizeof(seq_t));
    if (!dp) return 0;          /* memory failure – not expected */

    dp[0] = (seq_t)1;           /* base case: one way to make zero */

    /* Values of the parts allowed by the generating function. */
    const idx_t parts[4] = {6, 9, 10, 12};

    /* Standard coin‑change DP: for each part, add its contribution
       to all larger sums. */
    for (int p = 0; p < 4; ++p) {
        idx_t v = parts[p];
        if (v > n) continue;
        for (idx_t i = v; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - v];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
