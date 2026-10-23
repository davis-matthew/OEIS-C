/*
 * A025461  Number of partitions of n into 8 positive cubes.
 * https://oeis.org/A025461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into 8 positive cubes * a(n) =
 *            coefficient of x^n in the generating function * a(n) = [x^n]
 *            Product_{k>=1} 1/(1 - x^(k^3)) with exactly 8 parts
 * Range:     u8 n <= 9572, u16 n <= 9572, u32 n <= 9572, u64 n <= 9572,
 *            u128 n <= 9572, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* 9 rows in the DP table: j = 0..8 */
    size_t stride = (size_t)n + 1;
    size_t rows   = 9;
    size_t size   = stride * rows;

    /* Static buffer reused between calls */
    static seq_t *dp = NULL;
    static size_t dp_cap = 0;

    if (dp_cap < size) {
        /* Allocate larger buffer */
        void *p = realloc(dp, size * sizeof(seq_t));
        if (!p) return (seq_t)0;   /* Out of memory: return 0 */
        dp = (seq_t *)p;
        dp_cap = size;
    }

    /* Zero the used part of the table */
    memset(dp, 0, size * sizeof(seq_t));

    /* Base case: one way to form 0 using 0 cubes */
    dp[0 * stride + 0] = 1;

    /* Enumerate cube values up to n */
    for (idx_t k = 1;; ++k) {
        idx_t cube = k * k * k;
        if (cube > n) break;
        /* Update DP for this cube */
        for (int j = 1; j <= 8; ++j) {
            for (idx_t s = cube; s <= n; ++s) {
                dp[j * stride + s] += dp[(j - 1) * stride + (s - cube)];
            }
        }
    }

    return dp[8 * stride + n];
}
