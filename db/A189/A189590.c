/*
 * A189590  Number of permutations of 1..n with displacements restricted to
 *          {-5,-4,-2,0,1,3}.
 * https://oeis.org/A189590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n whose displacements belong
 *            to the set {-5,-4,-2,0,1,3}. Computed by a profile DP over the
 *            band of width 9 (offsets -5..3). The recurrence takes O(n·512)
 *            time and uses O(512) memory
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define WIDTH 9
#define MASK_SIZE (1 << WIDTH)
static const int OFFSETS[6] = { -5, -4, -2, 0, 1, 3 };

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; a(0) is undefined */

    seq_t dp[MASK_SIZE];
    seq_t ndp[MASK_SIZE];
    for (int i = 0; i < MASK_SIZE; ++i) dp[i] = 0;

    /* columns < 1 are considered already occupied; offsets -5..-1
       correspond to bit positions 0..4. */
    const int init_mask = (1 << 5) - 1;   /* 0b00011111 */
    dp[init_mask] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        for (int k = 0; k < MASK_SIZE; ++k) ndp[k] = 0;
        for (int mask = 0; mask < MASK_SIZE; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) continue;
            for (int s = 0; s < 6; ++s) {
                int bit = OFFSETS[s] + 5;        /* map offset to bit pos */
                if (mask & (1 << bit)) continue;/* already used */
                idx_t dest = i + OFFSETS[s];
                if (dest < 1 || dest > n) continue;
                int new_mask = (mask | (1 << bit)) >> 1;
                new_mask &= MASK_SIZE - 1;
                ndp[new_mask] += ways;
            }
        }
        for (int k = 0; k < MASK_SIZE; ++k) dp[k] = ndp[k];
    }

    /* After processing all rows, dp holds states for i = n+1.
       Columns 1..n must all be occupied in the current window. */
    int required_mask = 0;
    for (int o = -5; o <= 3; ++o) {
        idx_t col = (n + 1) + o;
        if (col < 1 || col > n) continue;
        required_mask |= 1 << (o + 5);
    }

    seq_t result = 0;
    for (int mask = 0; mask < MASK_SIZE; ++mask) {
        if ((mask & required_mask) == required_mask)
            result += dp[mask];
    }
    return result;
}
