/*
 * A219490  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 5, 2, 7, 9, 16, 25,
 *          ....
 * https://oeis.org/A219490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as sum of distinct
 *            elements of the generalized Fibonacci sequence starting 5, 2, 7,
 *            9, 16, …
 * Range:     u8 n <= 332, u16 n <= 8232, u32 n <= 8232, u64 n <= 8232,
 *            u128 n <= 8232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n) {
    /* Allocate DP array */
    seq_t *dp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;       /* allocation failure, return 0 as safe fallback */

    /* Initialise DP */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;               /* empty sum */

    /* Helper macro to process a single Fibonacci value */
    #define PROCESS(val)                          \
        do {                                      \
            idx_t fv = (idx_t)(val);              \
            if (fv <= n) {                        \
                for (idx_t sum = n; sum >= fv; --sum) { \
                    dp[sum] = dp[sum] + dp[sum - fv]; \
                }                                 \
            }                                      \
        } while (0)

    /* Process the initial two terms 5 and 2 */
    PROCESS(5);
    PROCESS(2);

    /* Generate the rest of the generalized Fibonacci sequence */
    seq_t f1 = 5, f2 = 2;   /* f1 = F(k-1), f2 = F(k) */
    while (1) {
        seq_t f_next = f1 + f2;
        if (f_next > (seq_t)n)
            break;
        f1 = f2;
        f2 = f_next;
        PROCESS(f2);
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
