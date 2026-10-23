/*
 * A188847  Number of nX5 binary arrays without the pattern 0 0 1 vertically,
 *          antidiagonally or horizontally.
 * https://oeis.org/A188847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by a transfer‑matrix DP. The state is the last two rows
 *            (each of width 5, avoiding the horizontal pattern 0‑0‑1).
 *            Transitions respect the vertical and anti‑diagonal patterns. The
 *            DP runs in O(n·400·20) time
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* All 5‑bit rows that avoid the horizontal pattern 0‑0‑1. */
static const uint16_t ROWS[20] = {
    0,  8, 10, 11, 12, 13, 14, 15, 16,
   20, 21, 22, 23, 24, 26, 27, 28, 29,
   30, 31
};

/* Transition tables: for each pair (prevprev,prev) we store the
   indices of valid next rows.  Transition list length per state. */
static uint16_t trans[400][20];
static uint8_t  trans_len[400];
static bool    trans_init = false;

/* Initialize the transition tables once. */
static void init_trans(void)
{
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            int idx = i * 20 + j;          /* state index (prevprev,prev) */
            int cnt = 0;
            int bits1 = ROWS[i];
            int bits2 = ROWS[j];
            for (int k = 0; k < 20; ++k) { /* candidate next row */
                int bits3 = ROWS[k];
                bool ok = true;

                /* Vertical check: no 0-0-1 in same column */
                for (int c = 0; c < 5; ++c) {
                    if (((bits1 >> c) & 1) == 0 &&
                        ((bits2 >> c) & 1) == 0 &&
                        ((bits3 >> c) & 1) == 1) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                /* Antidiagonal check: no 0-0-1 on (r-2,c), (r-1,c+1), (r,c+2) */
                for (int c = 0; c < 3; ++c) {
                    if (((bits1 >> c) & 1) == 0 &&
                        ((bits2 >> (c + 1)) & 1) == 0 &&
                        ((bits3 >> (c + 2)) & 1) == 1) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) continue;

                trans[idx][cnt++] = (uint16_t)k;
            }
            trans_len[idx] = (uint8_t)cnt;
        }
    }
    trans_init = true;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 1) return 20;   /* all 5‑bit rows */
    if (n == 2) return 400;  /* 20 × 20 pairs */

    /* Ensure the transition tables are ready. */
    if (!trans_init) init_trans();

    /* DP arrays: counts for each state (prevprev,prev). */
    seq_t cur[400] = {0};
    seq_t nxt[400];

    /* Base for length 2: each pair occurs once. */
    for (int s = 0; s < 400; ++s) cur[s] = 1;

    /* DP for rows 3 .. n. */
    for (idx_t step = 3; step <= n; ++step) {
        /* Zero the next vector. */
        for (int s = 0; s < 400; ++s) nxt[s] = 0;

        /* Transition from `cur` to `nxt`. */
        for (int s = 0; s < 400; ++s) {
            seq_t cnt = cur[s];
            if (cnt == 0) continue;
            int len = trans_len[s];
            int prev = s % 20;          /* second row of the pair */
            for (int t = 0; t < len; ++t) {
                int cur_row = trans[s][t];
                int next_state = prev * 20 + cur_row;
                nxt[next_state] += cnt;
            }
        }

        /* Prepare for next iteration. */
        for (int s = 0; s < 400; ++s) cur[s] = nxt[s];
    }

    /* Sum all counts for length n. */
    seq_t total = 0;
    for (int s = 0; s < 400; ++s) total += cur[s];
    return total;
}
