/*
 * A192096  Maximum number of tatami tilings of any m X m square region with
 *          exactly n horizontal dimers and m monomers.
 * https://oeis.org/A192096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * coefficient of x^n in ∏_{k≥1} (1 + 2·x^k + x^{2k})
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 109, u64 n <= 379,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              (equivalently, 2 · the coefficient of x^n in ∏_{k≥1} (1 + x^k)^2) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t sz = (size_t)(n + 1);
    /* dp[j]  : coefficient of x^j after processing factors for all k < current k */
    /* next[j] : coefficient of x^j after processing current k */
    seq_t *dp   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !next) {
        free(dp);
        free(next);
        return (seq_t)0;                     /* allocation failure */
    }

    dp[0] = (seq_t)1;                         /* product starts with 1 */

    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t j = 0; j <= n; ++j) {
            seq_t val = dp[j];
            if (j >= k)       val += (seq_t)2 * dp[j - k];  /* 2·x^k term */
            if (j >= 2 * k)   val += dp[j - 2 * k];        /* x^{2k} term */
            next[j] = val;
        }
        /* swap dp and next for next iteration */
        seq_t *tmp = dp;
        dp = next;
        next = tmp;
    }

    seq_t result = (seq_t)2 * dp[n];           /* final external factor 2 */
    free(dp);
    free(next);
    return result;
}
