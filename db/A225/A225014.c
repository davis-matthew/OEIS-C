/*
 * A225014  Number of 7Xn 0..1 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A225014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary arrays with rows unimodal and
 *            columns nondecreasing
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 22, u64 n <= 137, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225014.  The method uses a finite‑state automaton on the 7 row states (0=seen only zeros,
   1=in a block of ones, 2=finished ones and now only zeros).  Each column is represented by the
   position of the first 1 (cut) in the column, values 0..7.  Transitions are precomputed
   and a two‑array DP is performed for n columns. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ROWS 7
#define NUM_CUTS 8
#define STATE_SPACE 2187   /* 3^7 */

static int init_precomp = 0;
static const int pow3[NUM_ROWS] = {1,3,9,27,81,243,729};
static int trans[STATE_SPACE][NUM_CUTS];
static uint8_t digits[STATE_SPACE][NUM_ROWS];
static seq_t dp_cur[STATE_SPACE];
static seq_t dp_next[STATE_SPACE];

static void precompute(void)
{
    for (int s = 0; s < STATE_SPACE; ++s) {
        int tmp = s;
        for (int r = 0; r < NUM_ROWS; ++r) {
            digits[s][r] = tmp % 3;
            tmp /= 3;
        }
    }
    for (int s = 0; s < STATE_SPACE; ++s) {
        for (int k = 0; k < NUM_CUTS; ++k) {
            int ns = 0;
            int ok = 1;
            for (int r = 0; r < NUM_ROWS; ++r) {
                int cur = digits[s][r];
                int entry = (k < (r + 1)) ? 1 : 0; /* bit in this row */
                int nd;
                if (cur == 0) {
                    nd = entry;
                } else if (cur == 1) {
                    nd = (entry == 1) ? 1 : 2;
                } else { /* cur == 2 */
                    if (entry == 0) nd = 2;
                    else { ok = 0; break; }
                }
                ns += nd * pow3[r];
            }
            trans[s][k] = ok ? ns : -1;
        }
    }
}

seq_t a(idx_t n)
{
    if (!init_precomp) {
        precompute();
        init_precomp = 1;
    }

    /* initial state: all rows in state 0 */
    memset(dp_cur, 0, sizeof(dp_cur));
    dp_cur[0] = 1;

    seq_t *cur  = dp_cur;
    seq_t *next = dp_next;
    for (idx_t col = 0; col < n; ++col) {
        memset(next, 0, sizeof(dp_next));
        for (int s = 0; s < STATE_SPACE; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            for (int k = 0; k < NUM_CUTS; ++k) {
                int ns = trans[s][k];
                if (ns >= 0) {
                    next[ns] += val;
                }
            }
        }
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    seq_t ans = 0;
    for (int s = 0; s < STATE_SPACE; ++s) ans += cur[s];
    return ans;
}
