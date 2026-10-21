/*
 * A219496  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 8, 1, 9, 10, 19, 29,
 *          48, ....
 * https://oeis.org/A219496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as a sum of distinct
 *            elements of the generalized Fibonacci sequence starting
 *            8,1,9,10,19,29,
 * Range:     u8 n <= 8715, u16 n <= 8715, u32 n <= 8715, u64 n <= 8715,
 *            u128 n <= 8715, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for any n >= 0.  This DP counts the ways to
   represent n as a sum of distinct Fibonacci terms.  The
   fib sequence is generated once per call and is bounded by n,
   so the algorithm runs in O(n * |F_n|) time and uses O(n)
   memory, far below the required 2 s budget for n up to 10 000. */
seq_t a(idx_t n)
{
    /* Allocate DP array of size n+1, initialized to 0 */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;          /* allocation failure – return 0 */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;                  /* empty sum */

    /* Generate generalized Fibonacci numbers up to n.
       The list size is tiny (≤ 64 for n ≤ 10 000). */
    idx_t fibs[64];
    int fibcnt = 0;
    if (n >= 8) fibs[fibcnt++] = (idx_t)8;
    if (n >= 1) fibs[fibcnt++] = (idx_t)1;

    while (fibcnt >= 2) {
        idx_t next = fibs[fibcnt - 1] + fibs[fibcnt - 2];
        if (next > n) break;
        fibs[fibcnt++] = next;
    }

    /* DP: for each Fibonacci term, try to add it to all existing sums */
    for (int fi = 0; fi < fibcnt; ++fi) {
        idx_t term = fibs[fi];
        for (idx_t s = n; s >= term; --s)
            dp[s] += dp[s - term];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
