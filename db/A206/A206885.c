/*
 * A206885  Number of nX4 0..1 arrays avoiding 0 1 0 horizontally and 1 0 0
 *          vertically.
 * https://oeis.org/A206885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by dynamic programming over 4‑column binary
 *            matrices avoiding the horizontal pattern 010 in each row and the
 *            vertical pattern 1 0 0 in any column
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALLOWED_ROWS 12
#define STATE_NUM (ALLOWED_ROWS * ALLOWED_ROWS)

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    static bool init_done = false;
    static int rows[ALLOWED_ROWS];
    static unsigned char allow_cnt[STATE_NUM];
    static unsigned char allow_list[STATE_NUM][ALLOWED_ROWS];
    static seq_t curr[STATE_NUM];
    static seq_t nextArr[STATE_NUM];

    if (!init_done) {
        /* Generate rows that avoid horizontal 010 */
        int idx = 0;
        for (int pat = 0; pat < 16; ++pat) {
            bool ok = true;
            for (int pos = 0; pos < 2; ++pos) {
                if (((pat >> pos) & 7) == 0b010) {
                    ok = false;
                    break;
                }
            }
            if (ok) rows[idx++] = pat;
        }

        /* Precompute transitions respecting vertical 1 0 0 */
        for (int i = 0; i < ALLOWED_ROWS; ++i) {
            for (int j = 0; j < ALLOWED_ROWS; ++j) {
                int s = i * ALLOWED_ROWS + j;
                unsigned char cnt = 0;
                for (int k = 0; k < ALLOWED_ROWS; ++k) {
                    bool bad = false;
                    for (int col = 0; col < 4; ++col) {
                        int b1 = (rows[i] >> col) & 1;
                        int b2 = (rows[j] >> col) & 1;
                        int b3 = (rows[k] >> col) & 1;
                        if (b1 == 1 && b2 == 0 && b3 == 0) {
                            bad = true;
                            break;
                        }
                    }
                    if (!bad) {
                        allow_list[s][cnt++] = (unsigned char)k;
                    }
                }
                allow_cnt[s] = cnt;
            }
        }
        init_done = true;
    }

    /* Base cases */
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)144;

    /* DP initialization for two rows: all pairs counted once */
    for (int s = 0; s < STATE_NUM; ++s) curr[s] = (seq_t)1;

    seq_t result = 0;
    for (idx_t r = 3; r <= n; ++r) {
        /* Zero next array */
        for (int s = 0; s < STATE_NUM; ++s) nextArr[s] = (seq_t)0;

        /* Transition */
        for (int s = 0; s < STATE_NUM; ++s) {
            seq_t val = curr[s];
            if (val == (seq_t)0) continue;
            int j = s % ALLOWED_ROWS;
            unsigned char cnt = allow_cnt[s];
            for (int t = 0; t < cnt; ++t) {
                int k = allow_list[s][t];
                nextArr[j * ALLOWED_ROWS + k] += val;
            }
        }

        /* Sum to obtain a(r) */
        seq_t sum = (seq_t)0;
        for (int s = 0; s < STATE_NUM; ++s) sum += nextArr[s];

        /* Prepare for next iteration */
        for (int s = 0; s < STATE_NUM; ++s) curr[s] = nextArr[s];

        result = sum;
    }
    return result;
}
