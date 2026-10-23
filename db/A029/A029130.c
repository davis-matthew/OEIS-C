/*
 * A029130  Expansion of 1/((1-x)(1-x^8)(1-x^10)(1-x^11)).
 * https://oeis.org/A029130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of solutions to 8b + 10c + 11d <= n (equivalently,
 *            the coefficient of x^n in 1/((1-x)(1-x^8)(1-x^10)(1-x^11))
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The parts allowed by the generating function. */
    const idx_t parts[4] = {1, 8, 10, 11};

    /* Allocate a table for dynamic programming.  We use malloc so the
       function works in both C and C++ (no new or VLA). */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (dp == NULL)      /* allocation failed – return 0 as a safe value */
        return (seq_t)0;

    /* Base case: there is exactly one way to make 0 (use no coins). */
    dp[0] = (seq_t)1;

    /* Initialise the rest of the table to 0.  The use of a loop rather
       than memset keeps the code independent of the size of seq_t. */
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Classic coin‑change DP: for each part, add its contributions to all
       reachable indices.  This counts unordered representations, which
       matches the product expansion of the generating function. */
    for (int pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] = dp[i] + dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
