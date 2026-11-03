/*
 * A161032  Number of partitions of n into positive Fibonacci numbers where
 *          every part appears at least 8 times.
 * https://oeis.org/A161032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into positive Fibonacci numbers
 *            where every part appears at least 8 times. (Computed via dynamic
 *            programming over the generating function Π(1 + x^{8F} + x^{9F} +
 *            ...).)
 * Range:     u8 n <= 85, u16 n <= 239, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 98 terms through n = 872 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Allocate DP arrays */
    size_t N = (size_t)n;
    seq_t *dp = mem_new(seq_t, N + 1);
    seq_t *used = mem_new(seq_t, N + 1);
    if (!dp || !used) { /* allocation failure */
        if (dp) mem_free(dp);
        if (used) mem_free(used);
        return 0;
    }

    dp[0] = 1;  /* base case: one way to make sum 0 (empty partition) */

    /* Generate positive Fibonacci numbers <= n */
    idx_t f_prev = 1;   /* F(1) = 1 */
    idx_t f_curr = 2;   /* F(2) = 2 (we take only distinct values) */
    do {
        size_t f = (size_t)f_prev;
        if (f > 0 && f <= N) {
            mem_set(used, 0, (N + 1) * sizeof(seq_t));

            /* Add the mandatory 8 copies: dp[s - 8F] contributes to used[s] */
            size_t base = 8 * f;
            if (base <= N) {
                for (size_t s = base; s <= N; ++s) {
                    used[s] = dp[s - base];
                }

                /* Allow additional (>=0) copies: unlimited coin of F */
                for (size_t s = f; s <= N; ++s) {
                    used[s] += used[s - f];
                }
            }

            /* Combine with current DP: either skip this coin or use it */
            for (size_t s = 0; s <= N; ++s) {
                dp[s] += used[s];
            }
        }

        /* Next Fibonacci number */
        idx_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;

    } while (f_prev <= n && f_prev > 0);  /* f_prev > 0 guards against overflow */

    seq_t result = dp[N];

    /* Free memory */
    mem_free(dp);
    mem_free(used);
    return result;
}
