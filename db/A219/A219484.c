/*
 * A219484  Number of representations of n as a sum of distinct elements of
 *          the generalized Fibonacci sequence beginning 2, 9, 11, 20, 31, 51,
 *          ....
 * https://oeis.org/A219484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16347, u16 n <= 16347, u32 n <= 16347, u64 n <= 16347,
 *            u128 n <= 16347, big any n
 * Validated: standard, pass, 117 terms through n = 9121 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count representations of n as a sum of distinct elements of the
   generalized Fibonacci sequence 2, 9, 11, 20, 31, 51, ... .
   a(n) is the number of subsets of this sequence whose sum equals n. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Generate the generalized Fibonacci numbers up to n. */
    idx_t fibs[200];
    size_t fiblen = 0;

    if (n >= (idx_t)2) {
        fibs[fiblen++] = (idx_t)2;
    }
    if (n >= (idx_t)9) {
        fibs[fiblen++] = (idx_t)9;
    }
    if (fiblen >= 2) {
        idx_t prev = fibs[0];
        idx_t curr = fibs[1];
        while (1) {
            idx_t next = prev + curr;
            if (next > n) break;
            fibs[fiblen++] = next;
            prev = curr;
            curr = next;
        }
    }

    /* Allocate DP array. */
    size_t N = (size_t)n;
    seq_t *dp = mem_new(seq_t, (N + 1));
    if (!dp) return (seq_t)0;

    for (size_t i = 0; i <= N; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;   /* empty subset */

    for (size_t k = 0; k < fiblen; ++k) {
        idx_t term = fibs[k];
        size_t t = (size_t)term;
        for (size_t s = N; s >= t; --s) {
            dp[s] += dp[s - t];
        }
    }

    seq_t result = dp[N];
    mem_free(dp);
    return result;
}
