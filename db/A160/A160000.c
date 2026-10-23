/*
 * A160000  Number of partitions of n into parts occurring in
 *          '3x+1'-trajectory starting with n.
 * https://oeis.org/A160000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of partitions of n into parts that appear in the
 *            3x+1 trajectory starting from n
 * Range:     u8 n <= 23, u16 n <= 64, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* The returned type seq_t is supplied by the build system (uint8_t …, 
 * arbitrary‑precision class, etc.).  idx_t is an unsigned integer type.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* OEIS offset starts at 1 */

    /* Mark the numbers ≤ n that occur in the 3x+1 trajectory of n. */
    uint8_t *used = (uint8_t*)calloc((size_t)(n + 1), sizeof(uint8_t));
    if (!used) return 0;

    idx_t x = n;
    while (1) {
        if (x <= n) used[x] = 1;
        if (x == 1) break;
        if (x & 1)          /* odd */
            x = 3 * x + 1;
        else                /* even */
            x >>= 1;
    }

    /* DP array for partitions: dp[i] = number of restricted partitions of i. */
    seq_t *dp = (seq_t*)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp) { free(used); return 0; }

    for (idx_t i = 0; i <= n; ++i) dp[i] = 0;
    dp[0] = 1;

    for (idx_t part = 1; part <= n; ++part) {
        if (!used[part]) continue;
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(used);
    return result;
}
