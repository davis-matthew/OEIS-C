/*
 * A025429  Number of partitions of n into 5 nonzero squares.
 * https://oeis.org/A025429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = number of partitions of n into 5 nonzero squares
 * Range:     u8 n <= 1218, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
/* In C++ use new/delete for proper construction/destruction of seq_t. */
#define ALLOC_SEQ(n) (new seq_t[(n)+1]())
#define FREE_SEQ(p) (delete[] (p))
#else
/* In C use calloc/free. */
#include <stdlib.h>
#define ALLOC_SEQ(n) ((seq_t*)calloc((n)+1, sizeof(seq_t)))
#define FREE_SEQ(p) (free(p))
#endif

seq_t a(idx_t n)
{
    /* Allocate six arrays for the 0..5 square-count DP. */
    seq_t *dp[6];
    for (int k = 0; k < 6; ++k) {
        dp[k] = ALLOC_SEQ(n);
        if (!dp[k]) {                /* Out‑of‑memory fallback */
            for (int j = 0; j < k; ++j) FREE_SEQ(dp[j]);
            return 0;
        }
    }

    /* Base case: one way to sum zero using zero squares. */
    dp[0][0] = 1;

    /* Enumerate squares up to n. */
    for (idx_t i = 1; ; ++i) {
        idx_t sq = i * i;
        if (sq > n) break;

        /* Update DP for all counts of squares 1..5. */
        for (idx_t k = 1; k <= 5; ++k) {
            /* For each reachable sum s, add partitions using sq. */
            for (idx_t s = sq; s <= n; ++s) {
                dp[k][s] += dp[k - 1][s - sq];
            }
        }
    }

    /* Result for exactly five nonzero squares. */
    seq_t result = dp[5][n];

    /* Free allocated memory. */
    for (int k = 0; k < 6; ++k) {
        FREE_SEQ(dp[k]);
    }

    return result;
}
