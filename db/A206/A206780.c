/*
 * A206780  Number of nX3 0..1 arrays avoiding 0 0 1 horizontally and 0 1 1
 *          vertically.
 * https://oeis.org/A206780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over 2×3 pattern states; a(n) = number of n×3 0..1 arrays
 *            avoiding 0 0 1 horizontally and 0 1 1 vertically
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const uint8_t allowed_rows[7] = {
    0b000, 0b010, 0b011, 0b100, 0b101, 0b110, 0b111
};

static bool good_transition(uint8_t prev2, uint8_t prev1, uint8_t curr)
{
    for (int c = 0; c < 3; ++c) {
        int b2 = (prev2 >> c) & 1;
        int b1 = (prev1 >> c) & 1;
        int b0 = (curr   >> c) & 1;
        if (b2 == 0 && b1 == 1 && b0 == 1)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)7;  // 7 valid rows

    seq_t curr[49] = {0};
    seq_t next[49] = {0};

    for (int i = 0; i < 49; ++i)
        curr[i] = (seq_t)1;   // for two rows, all 49 pairs are possible

    for (idx_t row = 3; row <= n; ++row) {
        for (int i = 0; i < 49; ++i)
            next[i] = (seq_t)0;

        for (int pair_id = 0; pair_id < 49; ++pair_id) {
            seq_t ways = curr[pair_id];
            if (!ways) continue;

            uint8_t prev2 = (uint8_t)(pair_id / 7);
            uint8_t prev1 = (uint8_t)(pair_id % 7);
            uint8_t pattern2 = allowed_rows[prev2];
            uint8_t pattern1 = allowed_rows[prev1];

            for (int curr_idx = 0; curr_idx < 7; ++curr_idx) {
                uint8_t pattern0 = allowed_rows[curr_idx];
                if (!good_transition(pattern2, pattern1, pattern0))
                    continue;
                int new_pair_id = prev1 * 7 + curr_idx;
                next[new_pair_id] += ways;
            }
        }

        for (int i = 0; i < 49; ++i)
            curr[i] = next[i];
    }

    seq_t ans = (seq_t)0;
    for (int i = 0; i < 49; ++i)
        ans += curr[i];

    return ans;
}
