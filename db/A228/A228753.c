/*
 * A228753  Number of nX7 binary arrays with top left element equal to 1 and
 *          no two ones adjacent horizontally or antidiagonally.
 * https://oeis.org/A228753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays with top‑left element 1, no
 *            two 1’s adjacent horizontally or antidiagonally
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1 */

    /* Precompute the 34 admissible 7‑bit patterns (no horizontal 11). */
    static uint32_t masks[34];
    static uint8_t compat[34][34];
    static int  mask_cnt = 0;
    static int  initialized = 0;

    if (!initialized) {
        /* find all patterns */
        for (int m = 0; m < 128; ++m) {
            if ((m & (m >> 1)) == 0) {
                masks[mask_cnt++] = (uint32_t)m;
            }
        }
        /* compute compatibility matrix */
        for (int i = 0; i < mask_cnt; ++i) {
            uint32_t pm = masks[i];
            for (int j = 0; j < mask_cnt; ++j) {
                uint32_t cm = masks[j];
                compat[i][j] = (cm & (pm >> 1)) == 0 ? 1 : 0;
            }
        }
        initialized = 1;
    }

    /* DP over rows */
    seq_t dp_cur[34];
    seq_t dp_next[34];

    /* row 1: must have bit0 == 1 */
    for (int i = 0; i < mask_cnt; ++i) {
        dp_cur[i] = (masks[i] & 1U) ? (seq_t)1 : (seq_t)0;
    }

    for (idx_t r = 2; r <= n; ++r) {
        /* zero next */
        for (int i = 0; i < mask_cnt; ++i) dp_next[i] = 0;

        /* transition */
        for (int i = 0; i < mask_cnt; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            for (int j = 0; j < mask_cnt; ++j) {
                if (compat[i][j]) {
                    dp_next[j] += val;
                }
            }
        }

        /* swap */
        for (int i = 0; i < mask_cnt; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all states */
    seq_t total = 0;
    for (int i = 0; i < mask_cnt; ++i) {
        total += dp_cur[i];
    }
    return total;
}
