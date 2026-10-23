/*
 * A116403  Number of representations of n as a sum of distinct elements of
 *          the Fibonacci-type sequence beginning 4, 1, 5, 6, 11, ...
 * https://oeis.org/A116403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7346, u16 n <= 7346, u32 n <= 7346, u64 n <= 7346,
 *            u128 n <= 7346, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes a(n) = number of representations of n as a sum of distinct
    Fibonacci‑type numbers 4,1,5,6,11,… (each used at most once).
    The implementation uses subset‑sum dynamic programming.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* length of DP array */
    size_t len = (size_t)n + 1;

    /* allocate the DP array of seq_t elements */
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) {
        /* allocation failed – return zero as a safe fallback */
        return (seq_t)0;
    }

    /* initialise dp[0] = 1 (empty sum), others 0 */
    dp[0] = (seq_t)1;
    for (size_t i = 1; i < len; ++i) {
        dp[i] = (seq_t)0;
    }

    /* Fibonacci‑type numbers: 4,1,5,6,11,… */
    idx_t f1 = 4, f2 = 1;

    /* process first element (4) */
    if (f1 <= n) {
        for (size_t s = (size_t)n; s >= (size_t)f1; --s) {
            dp[s] += dp[s - (size_t)f1];
        }
    }

    /* process second element (1) */
    if (f2 <= n) {
        for (size_t s = (size_t)n; s >= (size_t)f2; --s) {
            dp[s] += dp[s - (size_t)f2];
        }
    }

    /* process remaining Fibonacci‑type numbers up to n */
    while (1) {
        idx_t fn = f1 + f2;        /* next number */
        if (fn > n) break;         /* stop if it exceeds n */

        for (size_t s = (size_t)n; s >= (size_t)fn; --s) {
            dp[s] += dp[s - (size_t)fn];
        }

        /* advance the sequence */
        f1 = f2;
        f2 = fn;
    }

    /* the desired count is at dp[n] */
    seq_t result = dp[(size_t)n];

    free(dp);
    return result;
}
