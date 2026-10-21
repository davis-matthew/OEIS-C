/*
 * A202490  Number of (n+2)X8 binary arrays avoiding patterns 000 and 010 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of (n+2)×8 binary arrays avoiding 000 and 010 vertically,
   horizontally and on NW‑to‑SE diagonals (OEIS A202490). 
   No closed form is known; the value is computed by a transfer‑matrix DP
   on the 64 admissible rows. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compile‑time constants for the width and state dimension. */
enum { COLS = 8, VALID_CNT = 64, STATE_CNT = VALID_CNT * VALID_CNT };

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       1.  Compute the 64 admissible rows (no 000 or 010 horizontally).
       ------------------------------------------------------------------ */
    static uint8_t rows[VALID_CNT];
    static bool rows_ready = false;
    if (!rows_ready) {
        int idx = 0;
        for (int val = 0; val < (1 << COLS); ++val) {
            bool good = true;
            for (int j = 0; j <= COLS - 3; ++j) {
                int a = (val >> j) & 1;
                int b = (val >> (j + 1)) & 1;
                int c = (val >> (j + 2)) & 1;
                if ((a == 0 && b == 0 && c == 0) ||
                    (a == 0 && b == 1 && c == 0)) {
                    good = false;
                    break;
                }
            }
            if (good) rows[idx++] = (uint8_t)val;
        }
        rows_ready = true;
    }

    /* ------------------------------------------------------------------
       2.  For each admissible row precompute:
           (a) mask[i] – bits that must be 1 in a succeeding row
                          (to forbid vertical 000/010).
           (b) cand[i][] – list of indices of admissible next rows
                          that satisfy the mask for row i.
        ------------------------------------------------------------------ */
    static uint8_t mask[VALID_CNT];
    static uint8_t cand[VALID_CNT][VALID_CNT];
    static size_t cand_len[VALID_CNT];
    static bool precomp = false;
    if (!precomp) {
        for (int i = 0; i < VALID_CNT; ++i) {
            uint8_t r = rows[i];
            uint8_t m = 0;
            for (int j = 0; j < COLS; ++j) {
                bool force = false;
                if (((r >> j) & 1) == 0) force = true;          /* vertical */
                if (j >= 2 && ((r >> (j - 2)) & 1) == 0) force = true; /* diagonal */
                if (force) m |= (1 << j);
            }
            mask[i] = m;

            /* Build candidate list for this prev2 row. */
            size_t len = 0;
            for (int k = 0; k < VALID_CNT; ++k) {
                uint8_t candrow = rows[k];
                if ((candrow & m) == m)
                    cand[i][len++] = (uint8_t)k;
            }
            cand_len[i] = len;
        }
        precomp = true;
    }

    /* ------------------------------------------------------------------
       3.  Dynamic programming over rows.
       ------------------------------------------------------------------ */
    seq_t *cur = (seq_t *)calloc(STATE_CNT, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(STATE_CNT, sizeof(seq_t));
    if (!cur || !nxt) {
        free(cur);
        free(nxt);
        return (seq_t)0;
    }

    /* Initial state: first two rows can be any admissible pair. */
    for (int r0 = 0; r0 < VALID_CNT; ++r0)
        for (int r1 = 0; r1 < VALID_CNT; ++r1)
            cur[r0 * VALID_CNT + r1] = (seq_t)1;

    /* Number of rows to add after the first two (total rows = n+2). */
    idx_t steps = n;

    for (idx_t step = 0; step < steps; ++step) {
        /* clear next array */
        for (int i = 0; i < STATE_CNT; ++i) nxt[i] = (seq_t)0;

        for (int prev2 = 0; prev2 < VALID_CNT; ++prev2) {
            size_t len   = cand_len[prev2];
            uint8_t *list = cand[prev2];
            int base = prev2 * VALID_CNT;
            for (int prev1 = 0; prev1 < VALID_CNT; ++prev1) {
                seq_t val = cur[base + prev1];
                if (val == (seq_t)0) continue;
                for (size_t ci = 0; ci < len; ++ci) {
                    int t = prev1 * VALID_CNT + list[ci];
                    nxt[t] += val;
                }
            }
        }
        /* swap arrays for next iteration */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;
    }

    /* Sum all final states to get total number of arrays. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_CNT; ++i) total += cur[i];

    free(cur);
    free(nxt);
    return total;
}
