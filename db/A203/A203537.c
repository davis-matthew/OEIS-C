/*
 * A203537  Number of nX3 0..2 arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A203537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A203537:
   Count of n×3 arrays over {0,1,2} where each cell has
   at least one horizontal/vertical neighbor that is 0 and
   at least one that is 1.
   Uses a transfer‑matrix DP with 1728 states per row.  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define STATE_PAT 27
#define STATE_MASK 64
#define STATE_CNT (STATE_PAT * STATE_MASK)

/* Helper to load statics once */
static void init_tables(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* pattern values: pattern[i][j] is the value (0..2) at column j of pattern i */
    static unsigned char pattern_val[STATE_PAT][3];
    for (int i = 0; i < STATE_PAT; ++i) {
        int t = i;
        for (int j = 0; j < 3; ++j) {
            pattern_val[i][j] = t % 3;
            t /= 3;
        }
    }

    /* horizontal neighbor bits: horbits[i][j] has bit0 if a left/right neighbor is 0,
                               bit1 if a left/right neighbor is 1 */
    static unsigned char horbits[STATE_PAT][3];
    for (int i = 0; i < STATE_PAT; ++i) {
        for (int j = 0; j < 3; ++j) {
            unsigned char bits = 0;
            if (j > 0) {
                unsigned char v = pattern_val[i][j - 1];
                if (v == 0) bits |= 1;
                else if (v == 1) bits |= 2;
            }
            if (j < 2) {
                unsigned char v = pattern_val[i][j + 1];
                if (v == 0) bits |= 1;
                else if (v == 1) bits |= 2;
            }
            horbits[i][j] = bits;
        }
    }

    /* transition table: trans[old_state][new_pattern] = new_state or 0xFFFF */
    static unsigned short trans[STATE_CNT][STATE_PAT];
    for (int old_state = 0; old_state < STATE_CNT; ++old_state) {
        int old_pat = old_state / STATE_MASK;
        int old_mask = old_state % STATE_MASK;
        for (int new_pat = 0; new_pat < STATE_PAT; ++new_pat) {
            bool ok = true;
            unsigned short new_mask = 0;
            for (int col = 0; col < 3; ++col) {
                int old_bits_col = (old_mask >> (2 * col)) & 3;
                unsigned char val_new = pattern_val[new_pat][col];
                unsigned char added_bits = (val_new == 0) ? 1 :
                                           (val_new == 1) ? 2 : 0;
                if ((old_bits_col | added_bits) != 3) {
                    ok = false;
                    break;
                }
                unsigned char new_bits = horbits[new_pat][col];
                unsigned char val_above = pattern_val[old_pat][col];
                if (val_above == 0) new_bits |= 1;
                else if (val_above == 1) new_bits |= 2;
                new_mask |= (new_bits << (2 * col));
            }
            if (ok)
                trans[old_state][new_pat] = (unsigned short)(new_pat * STATE_MASK + new_mask);
            else
                trans[old_state][new_pat] = 0xFFFF;
        }
    }

    /* expose statically by making them global */
    for (int i = 0; i < STATE_PAT; ++i)
        for (int j = 0; j < 3; ++j)
            pattern_val[i][j] = pattern_val[i][j];
    for (int i = 0; i < STATE_PAT; ++i)
        for (int j = 0; j < 3; ++j)
            horbits[i][j] = horbits[i][j];
}

static unsigned char pattern_val[STATE_PAT][3];
static unsigned char horbits[STATE_PAT][3];
static unsigned short trans[STATE_CNT][STATE_PAT];

/* The main function */
seq_t a(idx_t n)
{
    /* Ensure tables are initialized once */
    static bool tables_done = false;
    if (!tables_done) {
        /* Populate static tables */
        for (int i = 0; i < STATE_PAT; ++i) {
            int t = i;
            for (int j = 0; j < 3; ++j) {
                pattern_val[i][j] = t % 3;
                t /= 3;
            }
        }
        for (int i = 0; i < STATE_PAT; ++i) {
            for (int j = 0; j < 3; ++j) {
                unsigned char bits = 0;
                if (j > 0) {
                    unsigned char v = pattern_val[i][j - 1];
                    if (v == 0) bits |= 1;
                    else if (v == 1) bits |= 2;
                }
                if (j < 2) {
                    unsigned char v = pattern_val[i][j + 1];
                    if (v == 0) bits |= 1;
                    else if (v == 1) bits |= 2;
                }
                horbits[i][j] = bits;
            }
        }
        for (int old_state = 0; old_state < STATE_CNT; ++old_state) {
            int old_pat = old_state / STATE_MASK;
            int old_mask = old_state % STATE_MASK;
            for (int new_pat = 0; new_pat < STATE_PAT; ++new_pat) {
                bool ok = true;
                unsigned short new_mask = 0;
                for (int col = 0; col < 3; ++col) {
                    int old_bits_col = (old_mask >> (2 * col)) & 3;
                    unsigned char val_new = pattern_val[new_pat][col];
                    unsigned char added_bits = (val_new == 0) ? 1 :
                                               (val_new == 1) ? 2 : 0;
                    if ((old_bits_col | added_bits) != 3) {
                        ok = false;
                        break;
                    }
                    unsigned char new_bits = horbits[new_pat][col];
                    unsigned char val_above = pattern_val[old_pat][col];
                    if (val_above == 0) new_bits |= 1;
                    else if (val_above == 1) new_bits |= 2;
                    new_mask |= (new_bits << (2 * col));
                }
                if (ok)
                    trans[old_state][new_pat] = (unsigned short)(new_pat * STATE_MASK + new_mask);
                else
                    trans[old_state][new_pat] = 0xFFFF;
            }
        }
        tables_done = true;
    }

    if (n == 0) return 0;
    if (n == 1) return 0;  /* No array of size 1×3 satisfies the condition */

    seq_t *dp = (seq_t *)calloc(STATE_CNT, sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc(STATE_CNT, sizeof(seq_t));

    /* Initialize first row */
    for (int p = 0; p < STATE_PAT; ++p) {
        unsigned char mask = 0;
        for (int col = 0; col < 3; ++col)
            mask |= (horbits[p][col] << (2 * col));
        dp[p * STATE_MASK + mask] = 1;
    }

    /* Iterate rows 2..n */
    for (idx_t row = 2; row <= n; ++row) {
        memset(dp2, 0, STATE_CNT * sizeof(seq_t));
        for (int old_state = 0; old_state < STATE_CNT; ++old_state) {
            seq_t val = dp[old_state];
            if (val == 0) continue;
            for (int new_pat = 0; new_pat < STATE_PAT; ++new_pat) {
                unsigned short new_state = trans[old_state][new_pat];
                if (new_state != 0xFFFF) {
                    dp2[new_state] += val;
                }
            }
        }
        /* swap dp and dp2 */
        seq_t *tmp = dp;
        dp = dp2;
        dp2 = tmp;
    }

    /* Summation over valid final states (all cells already satisfied) */
    seq_t result = 0;
    for (int state = 0; state < STATE_CNT; ++state) {
        unsigned char mask = state % STATE_MASK;
        if (((mask >> 0) & 3) != 3) continue;
        if (((mask >> 2) & 3) != 3) continue;
        if (((mask >> 4) & 3) != 3) continue;
        result += dp[state];
    }

    free(dp);
    free(dp2);
    return result;
}
