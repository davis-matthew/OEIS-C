/*
 * A232073  Number of (n+1)X(5+1) 0..1 arrays with every element equal to some
 *          horizontal, diagonal or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: computed by dynamic programming over rows of a (n+1)×6 binary array,
 * where each element equals one of its horizontal, diagonal or antidiagonal
 * neighbors.  The top‑left element is fixed to 0.
 *  a(1) = 601
 *  a(2) = 26587
 *  a(n) = Σ_{prev_v,prev_unsat} a_{prev} * T[prev][cur]
 *  (see implementation for details)
 */
#include <stdint.h>
#include <stddef.h>

/* amount of states: 2^(6+6) = 4096 */
#define STATE_BITS   6
#define STATE_SIZE  (1 << STATE_BITS)          /* 64 */
#define STATES      (STATE_SIZE * STATE_SIZE)  /* 4096 */

/* maximum number of transitions per state (worst case) */
#define MAX_TRANS   64

static uint16_t trans_next[STATES][MAX_TRANS];
static uint8_t  trans_len[STATES];
static int      trans_ready = 0;

/* build the transition tables once */
static void build_transitions(void)
{
    if (trans_ready) return;
    for (int pv = 0; pv < STATE_SIZE; ++pv) {
        for (int pu = 0; pu < STATE_SIZE; ++pu) {
            int pidx = pv * STATE_SIZE + pu;
            int len  = 0;
            for (int cv = 0; cv < STATE_SIZE; ++cv) {
                /* check that all unsatisfied cells of previous row
                 * obtain a matching anti‑diagonal neighbor in the current row */
                int ok = 1;
                for (int j = 0; j < STATE_BITS; ++j) {
                    if ((pu >> j) & 1) {
                        int pv_val = (pv >> j) & 1;
                        int sat = 0;
                        if (j > 0 && ((cv >> (j - 1)) & 1) == pv_val) sat = 1;
                        else if (j < 5 && ((cv >> (j + 1)) & 1) == pv_val) sat = 1;
                        if (!sat) { ok = 0; break; }
                    }
                }
                if (!ok) continue;

                /* compute unsatisfied mask for current row */
                int cu = 0;
                for (int j = 0; j < STATE_BITS; ++j) {
                    int cv_val = (cv >> j) & 1;
                    int sat = 0;
                    /* horizontal */
                    if (j > 0 && ((cv >> (j - 1)) & 1) == cv_val) sat = 1;
                    else if (j < 5 && ((cv >> (j + 1)) & 1) == cv_val) sat = 1;
                    /* diagonal from previous row */
                    if (!sat) {
                        if (j > 0 && ((pv >> (j - 1)) & 1) == cv_val) sat = 1;
                        else if (!sat && j < 5 && ((pv >> (j + 1)) & 1) == cv_val) sat = 1;
                    }
                    if (!sat) cu |= 1 << j;
                }

                int cidx = cv * STATE_SIZE + cu;
                trans_next[pidx][len++] = (uint16_t)cidx;
            }
            trans_len[pidx] = (uint8_t)len;
        }
    }
    trans_ready = 1;
}

/* compute number of unsatisfied cells in the first row,
   considering only horizontal neighbors (no diagonals above) */
static int init_unsat_row0(int v)
{
    int unsat = 0;
    for (int j = 0; j < STATE_BITS; ++j) {
        int val = (v >> j) & 1;
        int sat = 0;
        if (j > 0 && ((v >> (j - 1)) & 1) == val) sat = 1;
        else if (j < 5 && ((v >> (j + 1)) & 1) == val) sat = 1;
        if (!sat) unsat |= 1 << j;
    }
    return unsat;
}

/* main function requested by the tests */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe */

    build_transitions();

    /* DP arrays: current and next row state counts */
    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];

    /* initialise with the very first row (row index 0) */
    for (size_t i = 0; i < (size_t)STATES; ++i) dp_cur[i] = (seq_t)0;
    for (int v = 0; v < STATE_SIZE; ++v) {
        if (v & 1) continue;          /* top‑left cell must be 0 */
        int unsat = init_unsat_row0(v);
        int idx = v * STATE_SIZE + unsat;
        dp_cur[idx] = (seq_t)1;
    }

    /* process rows 1 … n (inclusive) */
    for (idx_t row = 1; row <= n; ++row) {
        for (size_t i = 0; i < (size_t)STATES; ++i) dp_next[i] = (seq_t)0;
        for (int pidx = 0; pidx < STATES; ++pidx) {
            seq_t val = dp_cur[pidx];
            if (val == (seq_t)0) continue;
            int l = trans_len[pidx];
            for (int t = 0; t < l; ++t) {
                int cidx = trans_next[pidx][t];
                dp_next[cidx] += val;
            }
        }
        /* swap dp_cur and dp_next */
        for (size_t i = 0; i < (size_t)STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* after last row, only states with no unsatisfied cells contribute */
    seq_t result = (seq_t)0;
    for (int v = 0; v < STATE_SIZE; ++v) {
        int idx = v * STATE_SIZE;  /* unsatisfied mask = 0 */
        result += dp_cur[idx];
    }
    return result;
}
