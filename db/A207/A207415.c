/*
 * A207415  Number of n X 4 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=9, a(2)=81; for n≥3 counted by DP over last two rows
 *            (vertical and horizontal constraints)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)81;

    static const uint8_t allow[9] = {0, 6, 7, 8, 11, 12, 13, 14, 15};

    /* Transition tables: from pair (prev2,prev1) to allowed next rows. */
    static int next_list[256][9];
    static int next_cnt[256];
    static int initialized = 0;
    if (!initialized) {
        for (int s = 0; s < 256; ++s) next_cnt[s] = 0;
        for (int i = 0; i < 9; ++i) {
            int p = allow[i];
            for (int j = 0; j < 9; ++j) {
                int q = allow[j];
                unsigned state = (p << 4) | q;
                int idx = 0;
                for (int k = 0; k < 9; ++k) {
                    int r = allow[k];
                    int bad = 0;
                    for (int col = 0; col < 4; ++col) {
                        int b1 = (p >> col) & 1;
                        int b2 = (q >> col) & 1;
                        int b3 = (r >> col) & 1;
                        if ((b1 == 0 && b2 == 1 && b3 == 1) ||
                            (b1 == 1 && b2 == 0 && b3 == 1)) {
                            bad = 1;
                            break;
                        }
                    }
                    if (!bad) next_list[state][idx++] = r;
                }
                next_cnt[state] = idx;
            }
        }
        initialized = 1;
    }

    /* DP arrays: counts for each pair (prev2,prev1). */
    static seq_t cur[256];
    static seq_t nxt[256];
    for (int s = 0; s < 256; ++s) cur[s] = 0;

    /* Initialize for two rows: each allowed pair has count 1. */
    for (int i = 0; i < 9; ++i) {
        int p = allow[i];
        for (int j = 0; j < 9; ++j) {
            int q = allow[j];
            cur[(p << 4) | q] = (seq_t)1;
        }
    }

    /* Process rows 3..n. */
    for (idx_t row = 3; row <= n; ++row) {
        for (int s = 0; s < 256; ++s) nxt[s] = 0;
        for (int s = 0; s < 256; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;
            int cnt = next_cnt[s];
            if (cnt == 0) continue;
            int prev1 = s & 0xF;
            for (int k = 0; k < cnt; ++k) {
                int r = next_list[s][k];
                int new_state = (prev1 << 4) | r;
                nxt[new_state] += val;
            }
        }
        for (int s = 0; s < 256; ++s) cur[s] = nxt[s];
    }

    /* Sum all states to obtain the total number of matrices. */
    seq_t total = 0;
    for (int s = 0; s < 256; ++s) total += cur[s];
    return total;
}
