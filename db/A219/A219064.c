/*
 * A219064  Hilltop maps: number of 3Xn binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal, diagonal
 *          or antidiagonal neighbor in a random 0..2 3Xn array.
 * https://oeis.org/A219064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS A219064 – “Hilltop maps: number of 3×n binary arrays …” The
 *            sequence satisfies the empirical linear recurrence (for n > 10):
 *            a(n) = 7*a(n-1) + 6*a(n-2) +14*a(n-3) +10*a(n-4) +38*a(n-5)
 *            +23*a(n-6) -29*a(n-7) -52*a(n-8) -28*a(n-9) Initial values (a(1)
 *            … a(10)): 1, 59, 503, 4049, 32313, 258353,...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Base values a(1) … a(10) */
    static const seq_t base[10] = {
        (seq_t)1,          (seq_t)59,          (seq_t)503,
        (seq_t)4049,       (seq_t)32313,       (seq_t)258353,
        (seq_t)2066359,    (seq_t)16527691,    (seq_t)132195391,
        (seq_t)1057349609
    };

    /* For n <= 10, just return the stored value (index 1‑based) */
    if (n <= 10)
        return base[n - 1];

    /* Allocate space for terms up to n (1‑based index) */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* allocation failure – return 0 */

    /* Initialize the first 10 terms */
    for (idx_t i = 1; i <= 10; ++i)
        dp[i] = base[i - 1];

    /* Compute terms from 11 up to n using the recurrence */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t val = (seq_t)0;
        val += (seq_t)7  * dp[i - 1];
        val += (seq_t)6  * dp[i - 2];
        val += (seq_t)14 * dp[i - 3];
        val += (seq_t)10 * dp[i - 4];
        val += (seq_t)38 * dp[i - 5];
        val += (seq_t)23 * dp[i - 6];
        val -= (seq_t)29 * dp[i - 7];
        val -= (seq_t)52 * dp[i - 8];
        val -= (seq_t)28 * dp[i - 9];
        dp[i] = val;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
