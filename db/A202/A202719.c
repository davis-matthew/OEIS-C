/*
 * A202719  Number of (n+3)X4 binary arrays with no more than three of any
 *          consecutive four bits set in any row or column.
 * https://oeis.org/A202719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 14, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A202719:
   Number of (n+3)×4 binary arrays with no more than three of any consecutive
   four bits set in any row or column.
   Implementation uses dynamic programming over column run‑length states
   (2 bits per column for 4 columns → 256 states).  The DP counts how many
   rows can be appended without creating a vertical run of four 1’s.  All
   rows are processed, and the sum of the final state counts gives the
   required number. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Number of rows is n+3. */
static const idx_t ROWS_ADD = (idx_t)3;

/* Total number of column states: 2 bits per 4 columns (0..3 run length). */
#define STATES 256
/* Maximum number of distinct transitions for any state (≤15). */
#define MAX_TRANS 15

seq_t a(idx_t n)
{
    /* Pre‑compute the transition tables only once. */
    static bool init = false;
    static uint8_t trans_num[STATES];
    static uint8_t trans_dest[STATES][MAX_TRANS];
    static uint8_t trans_count[STATES][MAX_TRANS];

    if (!init) {
        for (int s = 0; s < STATES; ++s) {
            /* Temporary counter for each destination state. */
            uint16_t cnt[STATES] = {0};

            /* Enumerate all 4‑bit row patterns except 1111 */
            for (int p = 0; p < 16; ++p) {
                if (p == 15) continue;                /* row cannot be 1111 */
                int new_state = 0;
                int tmp = s;
                bool ok = true;
                for (int col = 0; col < 4; ++col) {
                    int run = tmp & 3;                 /* current run length for this column */
                    tmp >>= 2;
                    int bit = (p >> col) & 1;
                    if (bit) {
                        if (run == 3) { ok = false; break; } /* would create a vertical run of 4 ones */
                        run = run + 1;
                    } else {
                        run = 0;
                    }
                    new_state |= (run << (2 * col));
                }
                if (ok) {
                    cnt[new_state] += 1;
                }
            }
            /* Store non‑zero transitions for this state. */
            int nc = 0;
            for (int d = 0; d < STATES; ++d) {
                if (cnt[d]) {
                    trans_dest[s][nc]   = (uint8_t)d;
                    trans_count[s][nc]  = (uint8_t)cnt[d];
                    ++nc;
                }
            }
            trans_num[s] = (uint8_t)nc;
        }
        init = true;
    }

    /* Dynamic programming arrays. */
    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];
    for (int i = 0; i < STATES; ++i) dp_cur[i] = 0;
    dp_cur[0] = (seq_t)1;   /* initial state: all run lengths 0. */

    /* Process each of the (n+3) rows. */
    for (idx_t row = 0; row < (n + ROWS_ADD); ++row) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = 0;
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (!val) continue;
            int num = trans_num[s];
            for (int k = 0; k < num; ++k) {
                int dest  = trans_dest[s][k];
                seq_t add = val * (seq_t)trans_count[s][k];
                dp_next[dest] += add;
            }
        }
        /* Swap current and next arrays for next iteration. */
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all final states to obtain the total number of valid arrays. */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i) result += dp_cur[i];
    return result;
}
