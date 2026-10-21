/*
 * A204473  Number of 9-element subsets that can be chosen from
 *          {1,2,...,18*n+9} having element sum 81*n+45.
 * https://oeis.org/A204473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of 9‑element subsets of {1,2,…,18n+9} whose sum is
 *            81n+45
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 4, u64 n <= 77, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation method:  a(n) is the coefficient of q^(81n) in the Gaussian binomial
   coefficient [18n+9 choose 9]_q.  It is obtained by a small dynamic programme using
   the product representation of the q‑binomial:
/*
        [m choose k]_q = ∏_{i=1}^{k} (1 - q^{m-i+1}) / (1 - q^i) . 
   The algorithm multiplies successively by 1/(1-q^i) (by adding dp[s] += dp[s-i])
   and then by (1 - q^{m-i+1}) (by subtracting dp[s] -= dp[s-m+i-1]).
   The final value dp[81n] is the desired count.  The routine keeps a static
   buffer reused across calls (size grows as necessary).  Complexity:
   O(9 * 81n) ≈ 9·81·n ≈ 720 n operations – fast enough for n≤10 000. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Transform to the q‑binomial parameters */
    const seq_t M = (seq_t)18 * (seq_t)n + (seq_t)9;   /* M = 18n + 9 */
    const idx_t  t = (idx_t)81 * n;                    /* t = S - 45 = 81n */

    /* Static DP buffer reused across calls */
    static seq_t *dp = NULL;
    static idx_t  dp_size = 0;

    if (t + 1 > dp_size) {
        seq_t *tmp = (seq_t *)realloc(dp, (t + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        dp       = tmp;
        dp_size  = t + 1;
    }

    /* Initialise dp[0] = 1, others 0 */
    dp[0] = (seq_t)1;
    if (t > 0)
        memset(dp + 1, 0, t * sizeof(seq_t));

    /* Apply the q‑binomial product term by term */
    for (idx_t i = 1; i <= 9; ++i) {
        /* Multiply by 1/(1 - q^i)  → convolution with 1 + q^i + q^(2i) + ... */
        for (idx_t s = i; s <= t; ++s)
            dp[s] += dp[s - i];

        /* Multiply by (1 - q^(M-i+1)) → subtract shifted part */
        const idx_t shift = (idx_t)M - i + 1;   /* shift = M - i + 1 */
        if (shift <= t) {
            for (idx_t s = t; s >= shift; --s)
                dp[s] -= dp[s - shift];
        }
    }

    /* dp[t] now holds the coefficient of q^t which equals the required count */
    return dp[t];
}
