/*
 * A239003  Number of partitions of n into distinct Fibonacci numbers that are
 *          all greater than 2.
 * https://oeis.org/A239003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = number of partitions of n into distinct Fibonacci numbers
 *            > 2
 * Range:     u8 n <= 9274, u16 n <= 9274, u32 n <= 9274, u64 n <= 9274,
 *            u128 n <= 9274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* DP array for subset sum counts */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) return (seq_t)0;
    memset(dp, 0, (size_t)(n + 1) * sizeof(seq_t));
    dp[0] = 1;                     /* empty partition */

    /* Dynamic array to hold Fibonacci numbers > 2 up to n */
    size_t fib_cap = 8;
    idx_t *fibs = (idx_t *)malloc(fib_cap * sizeof(idx_t));
    if (!fibs) {
        free(dp);
        return (seq_t)0;
    }
    size_t fib_cnt = 0;

    /* Generate Fibonacci numbers starting from F4 = 3 */
    idx_t f_prev = 1, f_curr = 1;
    while (1) {
        idx_t f_next = f_prev + f_curr;
        if (f_next > n) break;            /* larger numbers cannot be used */
        if (f_next > 2) {                  /* we only need numbers > 2 */
            if (fib_cnt == fib_cap) {
                fib_cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(fibs, fib_cap * sizeof(idx_t));
                if (!tmp) {
                    free(fibs);
                    free(dp);
                    return (seq_t)0;
                }
                fibs = tmp;
            }
            fibs[fib_cnt++] = f_next;
        }
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* Count subsets of the Fibonacci numbers that sum to each value */
    for (size_t i = 0; i < fib_cnt; ++i) {
        idx_t f = fibs[i];
        for (idx_t s = n; s >= f; --s) {
            dp[s] += dp[s - f];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(fibs);
    return result;
}
