/*
 * A219495  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 7, 2, 9, 11, 20, 31,
 *          51, ....
 * https://oeis.org/A219495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1449, u16 n <= 9343, u32 n <= 9343, u64 n <= 9343,
 *            u128 n <= 9343, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) is the number of ways to represent n as a sum of
   distinct elements of the Fibonacci-like sequence starting 7, 2
   (i.e. 7, 2, 9, 11, 20, 31, …).  The algorithm uses a classic
   subset‑sum DP. */

#include <stdlib.h>

/* external types provided by the build system */
extern seq_t a(idx_t n)
{
    /* generate Fibonacci‑like numbers up to n */
    seq_t f1 = 7;
    seq_t f2 = 2;

    /* allocate DP array: dp[s] = #ways to form sum s */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;          /* allocation failure → 0 ways */

    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;                  /* empty sum */

    /* helper macro to process one Fibonacci number */
    #define PROC_F(VAL)                                    \
        if ((VAL) <= (seq_t)n) {                           \
            idx_t fidx = (idx_t)(VAL);                     \
            for (idx_t s = n; s >= fidx; --s)              \
                dp[s] += dp[s - fidx];                     \
        }

    /* process the first two numbers */
    PROC_F(f1);
    PROC_F(f2);

    /* continue generating until exceeding n */
    while (1) {
        seq_t f3 = f1 + f2;
        if (f3 > (seq_t)n) break;
        PROC_F(f3);
        f1 = f2;
        f2 = f3;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
