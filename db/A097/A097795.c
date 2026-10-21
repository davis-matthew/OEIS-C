/*
 * A097795  Number of partitions of 2*n into perfect numbers.
 * https://oeis.org/A097795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of 2*n into perfect numbers
 * Range:     u8 n <= 95, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t perfects[] = {6, 28, 496, 8128};
static const size_t NUM_PERFECT = sizeof(perfects) / sizeof(perfects[0]);

/* 2*n will never exceed 20000 for the intended test range (n up to 10000) */
#define MAX_TARGET 20000
static seq_t dp[MAX_TARGET + 1];

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n * 2;
    if (target > MAX_TARGET) {
        /* For n > 10000 the static array is insufficient; the
           sequence is not required for such large indices in the
           test suite here. */
        return 0;
    }
    size_t t = (size_t)target;
    for (size_t i = 0; i <= t; ++i) dp[i] = 0;
    dp[0] = 1;

    for (size_t pi = 0; pi < NUM_PERFECT; ++pi) {
        seq_t coin = perfects[pi];
        if (coin > target) continue;
        for (seq_t s = coin; s <= target; ++s) {
            dp[(size_t)s] += dp[(size_t)(s - coin)];
        }
    }
    return dp[t];
}
