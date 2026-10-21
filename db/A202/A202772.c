/*
 * A202772  Number of (n+2)X5 binary arrays avoiding patterns 000 and 011 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 26, u64 n <= 66, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A202772
 *
 *  Number of (n+2)×5 binary arrays avoiding patterns 000 and 011 in
 *  rows, columns and northwest‑to‑southeast diagonals.
 *
 *  Computation: transfer‑matrix DP over 5‑bit column vectors.
 *  States are ordered pairs of consecutive columns (a,b),
 *  where 0 ≤ a,b < 32.
 *  A column is *valid* if it contains no vertical 000 or 011 pattern.
 *  For each state (a,b) we pre‑compute the list of columns c such that
 *  the triple (a,b,c) satisfies the row and diagonal constraints.
 *  The DP starts with all pairs of valid columns (no constraints yet)
 *  and propagates N‑2 steps, where N = n+2 is the number of columns.
 *  This yields the correct value for any n ≥ 1 and works for
 *  arbitrary‑precision seq_t (as well as for built‑in integer types).
 */

#include <stdlib.h>
#include <string.h>

#define DP_STATES 1024          /* 32 × 32 */

static char valid_col[32];               /* 1 if column is vertical‑valid */
static unsigned char col_bits[32][5];    /* bits[row] of each column */

static unsigned char next_cnt[DP_STATES];        /* number of transitions for each state */
static unsigned char next_list[DP_STATES][32];   /* list of allowed next columns */

/* ------------------------------------------------------------------
 *  Initialise tables the first time a(...) is called.
 * ------------------------------------------------------------------ */
static void init_tables(void)
{
    static int done = 0;
    if (done)
        return;
    done = 1;

    /* compute valid columns and extract bit arrays */
    for (int v = 0; v < 32; ++v) {
        int ok = 1;
        for (int r = 0; r < 3; ++r) {
            int b0 = (v >> r)     & 1;
            int b1 = (v >> (r+1)) & 1;
            int b2 = (v >> (r+2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                (b0 == 0 && b1 == 1 && b2 == 1))
            {
                ok = 0;
                break;
            }
        }
        valid_col[v] = ok;
        for (int r = 0; r < 5; ++r)
            col_bits[v][r] = (unsigned char)((v >> r) & 1);
    }

    /* pre‑compute transitions */
    for (int a = 0; a < 32; ++a) {
        for (int b = 0; b < 32; ++b) {
            int idx = a * 32 + b;
            next_cnt[idx] = 0;
            if (!valid_col[a] || !valid_col[b])
                continue;

            for (int c = 0; c < 32; ++c) {
                if (!valid_col[c])
                    continue;

                int ok = 1;
                for (int r = 0; r < 5 && ok; ++r) {
                    int xb = col_bits[a][r];
                    int yb = col_bits[b][r];
                    int zb = col_bits[c][r];
                    if ((xb == 0 && yb == 0 && zb == 0) ||
                        (xb == 0 && yb == 1 && zb == 1))
                        ok = 0;
                }
                if (!ok)
                    continue;

                for (int r = 0; r < 3 && ok; ++r) {
                    int xb = col_bits[a][r];
                    int yb = col_bits[b][r+1];
                    int zb = col_bits[c][r+2];
                    if ((xb == 0 && yb == 0 && zb == 0) ||
                        (xb == 0 && yb == 1 && zb == 1))
                        ok = 0;
                }
                if (!ok)
                    continue;

                next_list[idx][next_cnt[idx]++] = (unsigned char)c;
            }
        }
    }
}

/* ------------------------------------------------------------------
 *  Public function required by the problem statement.
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* number of columns in the array */
    idx_t N = n + 2;
    if (N < 2)          /* should not happen for n ≥ 1 */
        return 0;

    init_tables();

    /* DP arrays for current and next column length */
    seq_t *dp_cur  = (seq_t *)calloc(DP_STATES, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(DP_STATES, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        free(dp_cur);
        free(dp_next);
        return 0;       /* out of memory → return 0 */
    }

    /* initialise with all pairs of valid columns (no constraints yet) */
    for (int a = 0; a < 32; ++a) {
        if (!valid_col[a])
            continue;
        for (int b = 0; b < 32; ++b) {
            if (!valid_col[b])
                continue;
            dp_cur[a*32 + b] = (seq_t)1;
        }
    }

    /* propagate over the remaining columns */
    for (idx_t step = 0; step < N - 2; ++step) {
        memset(dp_next, 0, DP_STATES * sizeof(seq_t));
        for (int s = 0; s < DP_STATES; ++s) {
            seq_t cnt = dp_cur[s];
            if (!cnt)
                continue;
            int b_col = s & 31;          /* second column of the pair */
            unsigned char tc = next_cnt[s];
            for (unsigned char i = 0; i < tc; ++i) {
                int c_col = next_list[s][i];
                int ns = b_col * 32 + c_col;
                dp_next[ns] = dp_next[ns] + cnt;
            }
        }
        /* swap pointers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* sum all final states to obtain the answer */
    seq_t result = 0;
    for (int s = 0; s < DP_STATES; ++s)
        result = result + dp_cur[s];

    free(dp_cur);
    free(dp_next);
    return result;
}
