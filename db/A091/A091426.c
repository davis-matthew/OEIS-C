/*
 * A091426  Expansion of (1 + x^18 + x^21 + x^22 + x^23 + x^24 + x^27 + x^45)
 *          / ( (1 - x^8)*(1 - x^12)*(1 - x^14)*(1 - x^15)).
 * https://oeis.org/A091426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence defined by the coefficient of x^n in the power series expansion
   of (1 + x^18 + x^21 + x^22 + x^23 + x^24 + x^27 + x^45) /
   ((1 - x^8)*(1 - x^12)*(1 - x^14)*(1 - x^15)) */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    /* parts allowed in the denominator */
    static const int parts[] = {8, 12, 14, 15};
    const int num_parts = 4;

    /* offsets coming from the numerator polynomial */
    static const int terms[] = {0, 18, 21, 22, 23, 24, 27, 45};
    const int num_terms = 8;

    /* allocation for dynamic programming table */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0; /* memory failure, return zero */

    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* classic coin‑change DP: count representations of m using parts */
    for (int pi = 0; pi < num_parts; ++pi) {
        int p = parts[pi];
        for (idx_t i = p; i <= n; ++i)
            dp[i] += dp[i - p];   /* dp[i] is seq_t, addition is fine */
    }

    /* accumulate contributions from numerator terms */
    seq_t result = 0;
    for (int ti = 0; ti < num_terms; ++ti) {
        int t = terms[ti];
        if (n >= (idx_t)t)
            result += dp[n - t];
    }

    free(dp);
    return result;
}
