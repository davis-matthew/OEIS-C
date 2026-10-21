/*
 * A230678  Number of 4Xn 0..2 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value 2-x(i,j).
 * https://oeis.org/A230678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Uses a transfer‑matrix DP. The state is a column (4 digits
 *            base‑3) and a bitmask of which rows in that column are already
 *            satisfied by a left neighbour. Transitions are pre‑computed. The
 *            DP runs in O(n·1296·81)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define COLS 81                 /* 3⁴ possible columns */
#define STATES (COLS * 16)      /* each column with 4‑bit mask */
#define MAX_TRANS 81            /* at most one transition per next column */

/* pre‑computed data */
static unsigned char col_vals[COLS][4]          /* digits of a column */
                    , maskRight[COLS][COLS]      /* cells in left column satisfied by right column */
                    , maskLeft[COLS][COLS]       /* cells in right column satisfied by left column */
                    , trans_count[STATES];
static unsigned short transitions[STATES][MAX_TRANS];
static bool precomp_done = false;

/* convert a column code to its 4 base‑3 digits */
static void init_col_vals(void)
{
    for (int code = 0; code < COLS; ++code) {
        int tmp = code;
        for (int r = 0; r < 4; ++r) {
            col_vals[code][r] = tmp % 3;
            tmp /= 3;
        }
    }
}

/* pre‑compute masks and transitions */
static void precompute(void)
{
    int i, j, r, s, delta;
    init_col_vals();

    /* masks between every pair of columns */
    for (i = 0; i < COLS; ++i) {
        for (j = 0; j < COLS; ++j) {
            unsigned char mr = 0, ml = 0;

            /* cells in column i satisfied by column j */
            for (r = 0; r < 4; ++r) {
                int val_i   = col_vals[i][r];
                int opp_i   = 2 - val_i;
                for (delta = -1; delta <= 1; ++delta) {
                    int nb = r + delta;
                    if (nb < 0 || nb > 3) continue;
                    if (col_vals[j][nb] == opp_i) {
                        mr |= 1u << r;
                        break;
                    }
                }
            }

            /* cells in column j satisfied by column i */
            for (s = 0; s < 4; ++s) {
                int val_j   = col_vals[j][s];
                int opp_j   = 2 - val_j;
                for (delta = -1; delta <= 1; ++delta) {
                    int nb = s + delta;
                    if (nb < 0 || nb > 3) continue;
                    if (col_vals[i][nb] == opp_j) {
                        ml |= 1u << s;
                        break;
                    }
                }
            }

            maskRight[i][j] = mr;
            maskLeft[i][j]  = ml;
        }
    }

    /* transitions */
    for (i = 0; i < COLS; ++i) {
        for (int prevMask = 0; prevMask < 16; ++prevMask) {
            unsigned short idxPrev = (unsigned short)(i * 16 + prevMask);
            trans_count[idxPrev] = 0;
            for (j = 0; j < COLS; ++j) {
                /* all rows in column i must be satisfied after adding column j */
                if ((prevMask | maskRight[i][j]) != 0x0F) continue;
                unsigned short nxt = (unsigned short)(j * 16 + maskLeft[i][j]);
                transitions[idxPrev][trans_count[idxPrev]++] = nxt;
            }
        }
    }

    precomp_done = true;
}

/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!precomp_done) precompute();

    static seq_t dp[STATES];
    static seq_t next[STATES];
    int k, t, col;

    /* initialise for the first column */
    for (k = 0; k < STATES; ++k) dp[k] = 0;
    for (col = 0; col < COLS; ++col) dp[col * 16 + 0] = 1;

    /* process remaining columns */
    for (idx_t step = 1; step < n; ++step) {
        for (k = 0; k < STATES; ++k) next[k] = 0;
        for (k = 0; k < STATES; ++k) {
            seq_t val = dp[k];
            if (!val) continue;
            unsigned char cnt = trans_count[k];
            for (t = 0; t < cnt; ++t) {
                unsigned short nxt = transitions[k][t];
                next[nxt] += val;
            }
        }
        for (k = 0; k < STATES; ++k) dp[k] = next[k];
    }

    /* sum states where last column is fully satisfied */
    seq_t result = 0;
    for (col = 0; col < COLS; ++col) {
        result += dp[col * 16 + 0x0F];
    }
    return result;
}
