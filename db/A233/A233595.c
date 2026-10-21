/*
 * A233595  Number of (n+2)X(1+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally, vertically or antidiagonally downwards.
 * https://oeis.org/A233595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A233595 via transfer‑matrix DP.
   a(n) counts (n+2)×3 arrays over {0,1,2} with no increasing sequence
   of length 3 horizontally, vertically or antidiagonally downward. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define COLS      3
#define BASE      3
#define PAT       (BASE * BASE * BASE)   /* 3³ = 27 row patterns          */
#define STATE     (PAT * PAT)           /* pair of last two rows → 729   */

seq_t a(idx_t n)
{
    /* ---------- static tables --------------------------------- */
    static int          initialized = 0;
    static unsigned char row_digits[PAT][COLS];
    static unsigned char allowed[PAT];
    static unsigned char allowed_cnt = 0;
    static uint16_t     adjacency[STATE][PAT];
    static unsigned char adj_len[STATE];

    if (!initialized)
    {
        /* pattern → column digits */
        for (int i = 0; i < PAT; ++i) {
            int v = i;
            row_digits[i][0] = v % BASE; v /= BASE;
            row_digits[i][1] = v % BASE; v /= BASE;
            row_digits[i][2] = v % BASE;
        }

        /* allowed rows – all except the horizontal 0,1,2 pattern (pattern 5) */
        allowed_cnt = 0;
        for (int i = 0; i < PAT; ++i) {
            if (!(row_digits[i][0] == 0 &&
                  row_digits[i][1] == 1 &&
                  row_digits[i][2] == 2)) {
                allowed[allowed_cnt++] = (unsigned char)i;
            }
        }

        /* build adjacency lists: (prevPrev,prev) → nxt */
        for (int s = 0; s < STATE; ++s) adj_len[s] = 0;

        for (int a1 = 0; a1 < allowed_cnt; ++a1) {
            int prevPrev = allowed[a1];
            for (int a2 = 0; a2 < allowed_cnt; ++a2) {
                int prev = allowed[a2];
                int s = prevPrev * PAT + prev;
                for (int a3 = 0; a3 < allowed_cnt; ++a3) {
                    int nxt = allowed[a3];

                    /* vertical 0,1,2 */
                    bool bad = false;
                    for (int c = 0; c < COLS; ++c) {
                        if (row_digits[prevPrev][c] == 0 &&
                            row_digits[prev][c]    == 1 &&
                            row_digits[nxt][c]     == 2) {
                            bad = true; break;
                        }
                    }
                    if (bad) continue;

                    /* antidiagonal 0,1,2 (cols 2→1→0) */
                    if (row_digits[prevPrev][2] == 0 &&
                        row_digits[prev][1]    == 1 &&
                        row_digits[nxt][0]     == 2) {
                        continue;
                    }

                    adjacency[s][adj_len[s]++] = (uint16_t)(prev * PAT + nxt);
                }
            }
        }
        initialized = 1;
    }

    /* ---------- DP: rows = n + 2 -------------------------------- */
    seq_t dp_cur[STATE];
    seq_t dp_nxt[STATE];
    for (int i = 0; i < STATE; ++i) dp_cur[i] = dp_nxt[i] = (seq_t)0;

    int rows = (int)(n + 2);   /* n ≥ 1 → rows ≥ 3 */

    /* initial two rows – all allowed pairs */
    for (int i = 0; i < allowed_cnt; ++i) {
        int r1 = allowed[i];
        for (int j = 0; j < allowed_cnt; ++j) {
            int r2 = allowed[j];
            dp_cur[r1 * PAT + r2] = (seq_t)1;
        }
    }

    /* add remaining rows (rows ≥ 3) */
    for (int r = 3; r <= rows; ++r)
    {
        /* zero next state array */
        for (int i = 0; i < STATE; ++i) dp_nxt[i] = (seq_t)0;

        for (int s = 0; s < STATE; ++s)
        {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            unsigned char len = adj_len[s];
            for (unsigned char k = 0; k < len; ++k)
            {
                int ns = adjacency[s][k];
                dp_nxt[ns] += val;
            }
        }

        /* swap dp_cur and dp_nxt */
        seq_t *tmp = dp_cur;
        for (int i = 0; i < STATE; ++i) dp_cur[i] = dp_nxt[i];
        /* no need to swap pointers because we copied dp_nxt into dp_cur
           and dp_nxt will be overwritten on next iteration */
    }

    /* sum all states to get the answer */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < STATE; ++i) ans += dp_cur[i];

    return ans;
}
