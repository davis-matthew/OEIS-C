/*
 * A035693  Number of partitions of n into parts 8k+3 and 8k+7 with at least
 *          one part of each type.
 * https://oeis.org/A035693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 190, u32 n <= 535, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = number of partitions of n into parts 8k+3 and 8k+7
 * with at least one part of each type.
 * The implementation uses a 2‑state DP: dp[i][mask] counts partitions of
 * i where mask indicates whether a 3‑type part (bit 0) and/or a 7‑type part
 * (bit 1) has been used.  The answer for n is dp[n][3] (both bits set).
 */
#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* limit the size of the DP table to avoid overflows */
    size_t sums = (size_t)(n + 1);
    if (sums > SIZE_MAX / 4) return 0;   /* graceful failure */

    /* Allocate the DP table: (n+1) rows × 4 masks */
#ifdef __cplusplus
    seq_t *dp = new seq_t[sums * 4]();   /* value‑initialized (zero) */
#else
    seq_t *dp = (seq_t *)calloc(sums * 4, sizeof(seq_t));
    if (!dp) return 0;
#endif

    dp[0] = (seq_t)1;                    /* dp[0][0] = 1 */

    const int BIT3 = 1;                  /* bit for parts 8k+3 */
    const int BIT7 = 2;                  /* bit for parts 8k+7 */

    /* Process parts ≡ 3 (mod 8) */
    for (idx_t part = 3; part <= n; part += 8) {
        for (idx_t i = part; i <= n; ++i) {
            size_t base = i * 4;
            size_t src  = (i - part) * 4;
            for (int mask = 0; mask < 4; ++mask) {
                seq_t val = dp[src + mask];
                if (val) {
                    int new_mask = mask | BIT3;
                    dp[base + new_mask] += val;
                }
            }
        }
    }

    /* Process parts ≡ 7 (mod 8) */
    for (idx_t part = 7; part <= n; part += 8) {
        for (idx_t i = part; i <= n; ++i) {
            size_t base = i * 4;
            size_t src  = (i - part) * 4;
            for (int mask = 0; mask < 4; ++mask) {
                seq_t val = dp[src + mask];
                if (val) {
                    int new_mask = mask | BIT7;
                    dp[base + new_mask] += val;
                }
            }
        }
    }

    seq_t result = dp[n * 4 + 3];         /* both types present */

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif

    return result;
}
