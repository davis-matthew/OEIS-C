/*
 * A104451  Number of representations of n as a sum of distinct elements of
 *          the Fibonacci-type sequence beginning 2, 5, 7, 12, 19, 31, 50, 81,
 *          ....
 * https://oeis.org/A104451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of representations of n as a sum of distinct
 *            elements of the Fibonacci-type sequence 2,5,7,12,... (F_0=2,
 *            F_1=5, F_k=F_{k-1}+F_{k-2})
 * Range:     u8 n <= 104, u16 n <= 9686, u32 n <= 9686, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   add_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/add_term.h"

seq_t a(idx_t n)
{
    /* Allocate and initialise DP table: dp[s] = number of ways to form sum s */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return 0;
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;                            /* empty subset sums to 0 */

    /* Fibonacci-type sequence: 2, 5, 7, 12, 19, ... */
    seq_t f0 = 2, f1 = 5;
    add_term(dp, n, f0);
    add_term(dp, n, f1);

    while (1) {
        seq_t f2 = f0 + f1;
        if (f2 > n) break;
        add_term(dp, n, f2);
        f0 = f1;
        f1 = f2;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
