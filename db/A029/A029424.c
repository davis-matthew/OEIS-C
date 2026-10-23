/*
 * A029424  Expansion of 1/((1-x^6)(1-x^8)(1-x^9)(1-x^11)).
 * https://oeis.org/A029424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to
 *            6*i+8*j+9*k+11*l = n, * i.e. the coefficient of x^n in
 *            1/((1-x^6)(1-x^8)(1-x^9)(1-x^11))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Parts used in the generating function */
    const idx_t parts[4] = {6, 8, 9, 11};

    /* Allocate DP array for coefficients up to n */
    size_t N = (size_t)n;               /* safe because n <= 10000 in typical use */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!dp) return 0;                  /* memory error: return 0 as a fallback */

    /* Initialize DP array */
    for (size_t i = 0; i <= N; ++i) dp[i] = 0;
    dp[0] = 1;                          /* base case: empty sum */

    /* Classic coin‑change DP: for each part, add its contributions */
    for (int p = 0; p < 4; ++p) {
        idx_t part = parts[p];
        if (part > N) continue;         /* skip if part larger than n */
        size_t start = (size_t)part;
        for (size_t i = start; i <= N; ++i)
            dp[i] += dp[i - start];
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
