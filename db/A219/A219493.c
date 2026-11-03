/*
 * A219493  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 6, 1, 7, 8, 15, 23,
 *          ....
 * https://oeis.org/A219493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 19685, u32 n <= 19685, u64 n <= 19685,
 *            u128 n <= 19685, big any n
 * Validated: standard, pass, 117 terms through n = 9619 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A219493
   Formula: a(n) = number of representations of n as a sum of distinct elements
   of the generalized Fibonacci sequence beginning 6, 1, 7, 8, 15, 23, ...
   We compute this by dynamic programming over the distinct Fibonacci-like numbers
   not exceeding n. */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* generate all Fibonacci-like numbers <= n */
    seq_t fibs[64];
    idx_t fib_cnt = 0;

    seq_t f0 = 6;  /* F(0) */
    seq_t f1 = 1;  /* F(1) */

    if ((idx_t)f0 <= n) fibs[fib_cnt++] = f0;
    if ((idx_t)f1 <= n) fibs[fib_cnt++] = f1;

    while (1) {
        seq_t f2 = f0 + f1;
        if ((idx_t)f2 > n) break;
        fibs[fib_cnt++] = f2;
        f0 = f1;
        f1 = f2;
    }

    /* dynamic programming array */
    seq_t *dp = mem_new(seq_t, (size_t)(n + 1));
    if (!dp) return (seq_t)0;          /* allocation failed – return 0 */
    for (size_t i = 0; i <= (size_t)n; ++i) dp[i] = 0;
    dp[0] = 1;                          /* empty sum */

    /* process each Fibonacci-like number as a distinct item */
    for (idx_t fi = 0; fi < fib_cnt; ++fi) {
        idx_t w = (idx_t)fibs[fi];      /* weight <= n */
        if (w > n) continue;
        /* iterate downward to guarantee distinctness */
        for (idx_t s = n; ; --s) {
            dp[s] += dp[s - w];
            if (s == w) break;
        }
    }

    seq_t result = dp[n];
    mem_free(dp);
    return result;
}
