/*
 * A207764  Number of 4Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   4×n binary arrays avoiding 000,101 horizontally and 011,101
 *            vertically Recurrence equivalent to a walk over a 3‑column
 *            automaton. a(1) = 9, a(2) = 81, otherwise computed by DP with 81
 *            states
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const int allowed[9] = {0,1,2,4,8,9,12,14,15};  // 4-bit columns satisfying vertical constraints

static bool initialized = false;
static int neigh_cnt[81];
static int neigh_idx[81][9];

static void init_adj(void) {
    for (int p = 0; p < 9; ++p) {
        for (int q = 0; q < 9; ++q) {
            int sid = p * 9 + q;
            neigh_cnt[sid] = 0;
            int patternP = allowed[p];
            int patternQ = allowed[q];
            for (int r = 0; r < 9; ++r) {
                int patternR = allowed[r];
                bool ok = true;
                for (int row = 0; row < 4; ++row) {
                    int bitP = (patternP >> (3 - row)) & 1;
                    int bitQ = (patternQ >> (3 - row)) & 1;
                    int bitR = (patternR >> (3 - row)) & 1;
                    if ((bitP == 0 && bitQ == 0 && bitR == 0) ||
                        (bitP == 1 && bitQ == 0 && bitR == 1)) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    neigh_idx[sid][neigh_cnt[sid]++] = q * 9 + r;
                }
            }
        }
    }
}

seq_t a(idx_t n) {
    if (!initialized) { init_adj(); initialized = true; }

    if (n == 1) return (seq_t)9;          /* single column: any allowed pattern */
    if (n == 2) return (seq_t)81;         /* any pair of allowed columns */

    seq_t dp_prev[81], dp_cur[81];
    for (int i = 0; i < 81; ++i) dp_prev[i] = 1;   /* all pairs for n = 2 */

    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < 81; ++i) dp_cur[i] = 0;
        for (int s = 0; s < 81; ++s) {
            seq_t val = dp_prev[s];
            if (val == 0) continue;
            int cnt = neigh_cnt[s];
            for (int k = 0; k < cnt; ++k) {
                int ns = neigh_idx[s][k];
                dp_cur[ns] += val;
            }
        }
        for (int i = 0; i < 81; ++i) dp_prev[i] = dp_cur[i];
    }

    seq_t result = 0;
    for (int i = 0; i < 81; ++i) result += dp_prev[i];
    return result;
}
