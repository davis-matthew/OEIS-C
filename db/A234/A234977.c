/*
 * A234977  Number of (n+1) X (3+1) 0..4 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of the edge differences
 *          equal to 8.
 * https://oeis.org/A234977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1) × 4 arrays over {0,1,2,3,4} such that
 *            every 2×2 subblock has the sum of absolute edge differences
 *            equal to 8
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum { STATE_COUNT = 625 };                // 5^4 possible rows

static uint8_t  row_val[STATE_COUNT][4];   // digits of each state
static uint16_t adj[STATE_COUNT][STATE_COUNT]; // adjacency list
static uint16_t adj_cnt[STATE_COUNT];          // number of neighbors
static bool adj_init = false;

//----- build adjacency ---------------------------------------------------------
static void compute_adj(void)
{
    if (adj_init) return;

    // encode each state into 4 base‑5 digits
    for (int id = 0; id < STATE_COUNT; ++id) {
        int tmp = id;
        for (int k = 0; k < 4; ++k) {
            row_val[id][k] = (uint8_t)(tmp % 5);
            tmp /= 5;
        }
    }

    // pre‑compute legal transitions between rows
    for (int s = 0; s < STATE_COUNT; ++s) {
        uint16_t cnt = 0;
        for (int t = 0; t < STATE_COUNT; ++t) {
            bool ok = true;
            for (int j = 0; j < 3 && ok; ++j) {
                int dt = abs((int)row_val[s][j]     - (int)row_val[s][j+1]); // top
                int db = abs((int)row_val[t][j]     - (int)row_val[t][j+1]); // bottom
                int d1 = abs((int)row_val[s][j]     - (int)row_val[t][j]);   // left
                int d2 = abs((int)row_val[s][j+1]   - (int)row_val[t][j+1]); // right
                if (dt + db + d1 + d2 != 8) ok = false;
            }
            if (ok) {
                adj[s][cnt] = (uint16_t)t;
                cnt++;
            }
        }
        adj_cnt[s] = cnt;
    }

    adj_init = true;
}

//----- main sequence function ----------------------------------------------------
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    compute_adj();

    seq_t dp_cur[STATE_COUNT];
    seq_t dp_next[STATE_COUNT];

    // first row can be any of the 625 states
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = (seq_t)1;

    seq_t answer = (seq_t)0;

    for (idx_t step = 1; step <= n; ++step) {
        // zero dp_next
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = (seq_t)0;

        // transition from current row to next row
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            uint16_t cnt = adj_cnt[s];
            for (uint16_t k = 0; k < cnt; ++k) {
                uint16_t t = adj[s][k];
                dp_next[t] = dp_next[t] + val;
            }
        }

        // sum all possibilities for this step -> a(step)
        answer = (seq_t)0;
        for (int t = 0; t < STATE_COUNT; ++t) {
            answer = answer + dp_next[t];
        }

        // prepare for next iteration
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    return answer;   // a(n)
}
