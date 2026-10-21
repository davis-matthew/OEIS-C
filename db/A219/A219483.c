/*
 * A219483  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 2, 7, 9, 16, 25, 41,
 *          ....
 * https://oeis.org/A219483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of representations of n as a sum of distinct
 *            elements of the generalized Fibonacci sequence beginning 2, 7,
 *            9, 16, 25, 41, …, where the sequence satisfies f(0)=2, f(1)=7
 *            and f(k)=f(k-1)+f(k-2) for k≥2
 * Range:     u8 n <= 9091, u16 n <= 9091, u32 n <= 9091, u64 n <= 9091,
 *            u128 n <= 9091, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /*  The empty sum counts for n = 0, as requested. */
    if (n == 0) return 1;

    /* Allocate an array to count ways to form each sum up to n. */
    seq_t *dp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;  /* allocation failed */

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    /* Generate the Fibonacci numbers that are ≤ n. */
    idx_t fibs[20];      /* 20 is ample for n ≤ 10 000 (the 17th term is 8129). */
    size_t fibcnt = 0;

    const idx_t f0 = 2;
    const idx_t f1 = 7;

    if (f0 <= n) fibs[fibcnt++] = f0;
    if (f1 <= n) fibs[fibcnt++] = f1;

    while (1) {
        if (fibcnt < 2) break;
        /* Avoid unsigned overflow when computing the next Fibonacci number. */
        if (fibs[fibcnt - 2] > UINT64_MAX - fibs[fibcnt - 1]) break;
        idx_t fnext = fibs[fibcnt - 2] + fibs[fibcnt - 1];
        if (fnext > n) break;
        fibs[fibcnt++] = fnext;
    }

    /* Count representations using each Fibonacci number exactly once. */
    for (size_t k = 0; k < fibcnt; ++k) {
        idx_t f = fibs[k];
        for (idx_t i = n; i >= f; --i) {
            dp[i] += dp[i - f];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
