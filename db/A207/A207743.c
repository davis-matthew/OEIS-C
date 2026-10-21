/*
 * A207743  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   is_valid_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A207743: Number of 5×n binary arrays avoiding
   horizontally 000 and 010, and vertically 001 and 100.
   The algorithm uses a transfer‑matrix DP over pairs of columns.
   State = (previous column, current column).  There are at most
   14 valid columns, so at most 196 states.  Each step adds a new
   column respecting the forbidden 3‑cell patterns.  Complexity
   O(n * 196 * 14). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_valid_col.h"

/* Count of valid columns and list of them. */
static int num_valid_cols;
static uint32_t valid_cols[32];

/* Transition data: for each state (p1,p2) store list of allowed third columns.
   [1024][14] is enough; most unused entries remain 0. */
static int trans_list[1024][14];
static int trans_cnt[1024];
static bool state_ok[1024];

/* DP arrays, reused between calls. */
static seq_t dp_arr[1024];
static seq_t next_arr[1024];

/* Pre‑compute valid columns and transition table. */
static void init_tables(void)
{
    static bool inited = false;
    if (inited) return;
    inited = true;

    /* Find all valid columns. */
    num_valid_cols = 0;
    for (uint32_t col = 0; col < 32U; ++col) {
        if (is_valid_col(col)) {
            valid_cols[num_valid_cols++] = col;
        }
    }

    /* Build transition table. */
    for (int i = 0; i < 1024; ++i) trans_cnt[i] = 0;
    for (int s = 0; s < 1024; ++s) state_ok[s] = false;

    for (int idx1 = 0; idx1 < num_valid_cols; ++idx1) {
        uint32_t c1 = valid_cols[idx1];
        for (int idx2 = 0; idx2 < num_valid_cols; ++idx2) {
            uint32_t c2 = valid_cols[idx2];
            int state = (c1 << 5) | c2;   /* encode as c1*32 + c2 */
            state_ok[state] = true;

            /* Determine allowed third columns. */
            for (int idx3 = 0; idx3 < num_valid_cols; ++idx3) {
                uint32_t c3 = valid_cols[idx3];
                bool ok = true;
                for (int r = 0; r < 5; ++r) {
                    uint32_t a = (c1 >> r) & 1U;
                    uint32_t b = (c2 >> r) & 1U;
                    uint32_t c = (c3 >> r) & 1U;
                    if (a == 0 && b == 0 && c == 0) { ok = false; break; }
                    if (a == 0 && b == 1 && c == 0) { ok = false; break; }
                }
                if (ok) {
                    trans_list[state][trans_cnt[state]++] = c3;
                }
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_tables();

    if (n == 1) {
        /* Only vertical constraints matter. */
        uint64_t cnt = (uint64_t)num_valid_cols;
        return (seq_t)cnt;
    }
    if (n == 2) {
        /* No horizontal patterns yet. */
        uint64_t cnt = (uint64_t)num_valid_cols * (uint64_t)num_valid_cols;
        return (seq_t)cnt;
    }

    /* Initialize DP for first two columns. */
    for (int i = 0; i < 1024; ++i) dp_arr[i] = 0;
    for (int idx1 = 0; idx1 < num_valid_cols; ++idx1) {
        uint32_t c1 = valid_cols[idx1];
        for (int idx2 = 0; idx2 < num_valid_cols; ++idx2) {
            uint32_t c2 = valid_cols[idx2];
            int state = (c1 << 5) | c2;
            dp_arr[state] = dp_arr[state] + 1;
        }
    }

    /* DP iterations for columns 3 .. n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* Clear next_arr */
        for (int i = 0; i < 1024; ++i) next_arr[i] = 0;

        for (int s = 0; s < 1024; ++s) {
            if (!state_ok[s]) continue;
            seq_t ways = dp_arr[s];
            if (ways == 0) continue;
            uint32_t c1 = s >> 5;
            uint32_t c2 = s & 31U;
            int tcnt = trans_cnt[s];
            for (int t = 0; t < tcnt; ++t) {
                uint32_t c3 = trans_list[s][t];
                int new_state = (c2 << 5) | c3;
                next_arr[new_state] = next_arr[new_state] + ways;
            }
        }
        /* Swap dp_arr and next_arr. */
        for (int i = 0; i < 1024; ++i) dp_arr[i] = next_arr[i];
    }

    /* Sum all ways for final columns. */
    seq_t total = 0;
    for (int i = 0; i < 1024; ++i) {
        total = total + dp_arr[i];
    }
    return total;
}
