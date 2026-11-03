/*
 * A034147  Number of partitions of n into distinct parts from [1, 17].
 * https://oeis.org/A034147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into distinct parts from [1,
 *            17]
 * Range:     u8 n <= 31, u16 n <= 153, u32 n <= 153, u64 n <= 153,
 *            u128 n <= 153, big any n
 * Validated: standard, pass, 73 terms through n = 148 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static const idx_t MAX_PART = 17;
static const idx_t MAX_SUM   = MAX_PART * (MAX_PART + 1) / 2;   /* 153 */
static seq_t *dp = NULL;

/* Initialize the DP table once */
static void init_dp(void)
{
    size_t i, j;
    dp = mem_new(seq_t, (MAX_SUM + 1));
    if (!dp) return;                 /* allocation failure should not happen */
    for (i = 0; i <= (size_t)MAX_SUM; ++i) dp[i] = 0;
    dp[0] = 1;                       /* empty subset */

    for (i = 1; i <= (size_t)MAX_PART; ++i) {
        seq_t coin = (seq_t)i;
        for (j = MAX_SUM; j >= i; --j) {
            dp[j] += dp[j - i];
        }
    }
}

seq_t a(idx_t n)
{
    if (dp == NULL) init_dp();
    if (dp == NULL) return 0; /* fallback in case of allocation failure */

    return (n <= (idx_t)MAX_SUM) ? dp[n] : 0;
}
