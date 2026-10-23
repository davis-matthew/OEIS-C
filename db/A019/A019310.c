/*
 * A019310  Number of words of length n (n >= 1) over a two-letter alphabet
 *          having a minimal period of size n-1.
 * https://oeis.org/A019310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n==1, a(2)=2, a(3)=2; for n>=3, a(n) =
 *            2*a(n-1)+(-1)^n * a(ceil(n/2))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses dynamic programming and is O(n) time */

/* We assume seq_t and idx_t are defined by the build.
   No external libraries are required for this implementation. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return value directly for the trivial case. */
    if (n == 1) return 0;
    if (n == 2 || n == 3) return 2;

    /* Allocate a cache array of size n+1. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;          /* unlikely but defensive */

    /* Base initialisations. */
    dp[0] = 0;                  /* unused but safe */
    dp[1] = 0;
    dp[2] = 2;
    dp[3] = 2;

    /* Compute subsequent terms. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t twice = dp[i - 1] + dp[i - 1];          /* 2 * a(n-1) */
        idx_t k = (i + 1) / 2;                       /* ceil(i/2) */
        if ((i & 1) == 0) {                          /* i even: + */
            dp[i] = twice + dp[k];
        } else { /* i odd: - */
            dp[i] = twice - dp[k];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
