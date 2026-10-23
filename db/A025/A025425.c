/*
 * A025425  Number of partitions of n into 10 squares.
 * https://oeis.org/A025425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into exactly 10 squares
 *            (including 0²)
 * Range:     u8 n <= 158, u16 n <= 1238, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert the (potentially 64‑bit) index to a size_t for array sizing. */
    size_t N = (size_t)n;
    size_t dim = N + 1;
    size_t total = 11 * dim;          /* 11 rows: k = 0 … 10 */

    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[total]();          /* value–initialized to zero */
#else
    dp = calloc(total, sizeof(seq_t)); /* zero-initialized */
#endif

    /* Base case: one way to partition 0 into 0 squares. */
    dp[0 * dim + 0] = (seq_t)1;

    /* Iterate over all positive squares up to n. */
    for (uint64_t i = 1; i * i <= n; ++i) {
        size_t val = (size_t)(i * i);   /* square value as size_t */

        for (int k = 1; k <= 10; ++k) {
            size_t k_off  = k  * dim;
            size_t k1_off = (k-1) * dim;
            for (size_t s = val; s <= N; ++s) {
                dp[k_off + s] += dp[k1_off + s - val];
            }
        }
    }

    /* Sum counts with 0 … 10 non‑zero squares; zeros pad to 10 parts. */
    seq_t result = (seq_t)0;
    for (int k = 0; k <= 10; ++k) {
        result += dp[k * dim + N];
    }

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
