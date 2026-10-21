/*
 * A230522  Number of nX4 0..2 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: DP transfer‑matrix counts n×4 arrays with the adjacency
   constraint that each cell has a neighbor (horizontally, vertically,
   diagonally or antidiagonally) with value (x+1) mod 3.  The upper‑left
   cell is forced to 0.  No direct formula is used. */

#include <stdbool.h>

static unsigned char patterns[81][4];       /* all 3⁴ row patterns */
static unsigned char alone_unsat[81];       /* mask of unsatisfied cells in a row alone */

/* Transition tables:
   trans_pattern[s][k] -> next row pattern index (0..80)
   trans_mask[s][k]     -> unsatisfied mask for that next row
   trans_count[s]       -> number of transitions for state s
*/
static unsigned char trans_pattern[1296][81];
static unsigned char trans_mask[1296][81];
static unsigned char trans_count[1296];
static bool initialized = false;

/* ---------- Initialization routine ----------------------------------- */
static void init(void) {
    /* generate all 81 patterns (base‑3 enumeration) */
    for (int i = 0; i < 81; ++i) {
        int x = i;
        for (int j = 0; j < 4; ++j) {
            patterns[i][j] = x % 3;
            x /= 3;
        }
    }

    /* mask of cells that lack a +1 neighbour in the same row */
    for (int p = 0; p < 81; ++p) {
        unsigned char mask = 0;
        for (int j = 0; j < 4; ++j) {
            unsigned char v = patterns[p][j];
            unsigned char need = (v + 1) % 3;
            bool has = false;
            if (j > 0 && patterns[p][j-1] == need) has = true;
            if (j < 3 && patterns[p][j+1] == need) has = true;
            if (!has) mask |= (1 << j);
        }
        alone_unsat[p] = mask;
    }

    /* pre‑compute all state transitions */
    for (int p = 0; p < 81; ++p) {
        for (int maskp = 0; maskp < 16; ++maskp) {
            int s = p * 16 + maskp;
            trans_count[s] = 0;
            for (int q = 0; q < 81; ++q) {
                /* check that every unsatisfied cell in row p is satisfied
                   by row q (below row) */
                bool ok = true;
                for (int j = 0; j < 4 && ok; ++j) {
                    if (maskp & (1 << j)) {
                        unsigned char need = (patterns[p][j] + 1) % 3;
                        bool sat = false;
                        if (j > 0 && patterns[q][j-1] == need) sat = true;
                        if (patterns[q][j]      == need) sat = true;
                        if (j < 3 && patterns[q][j+1] == need) sat = true;
                        if (!sat) ok = false;
                    }
                }
                if (!ok) continue;

                /* compute unsatisfied mask for row q after considering
                   neighbors above (row p) and within row q */
                unsigned char maskq = 0;
                for (int j = 0; j < 4; ++j) {
                    unsigned char vq = patterns[q][j];
                    unsigned char need = (vq + 1) % 3;
                    bool has = false;
                    if (j > 0 && patterns[q][j-1] == need) has = true;
                    if (j < 3 && patterns[q][j+1] == need) has = true;
                    if (j > 0 && patterns[p][j-1] == need) has = true;
                    if (patterns[p][j]      == need) has = true;
                    if (j < 3 && patterns[p][j+1] == need) has = true;
                    if (!has) maskq |= (1 << j);
                }

                trans_pattern[s][trans_count[s]] = (unsigned char)q;
                trans_mask[s][trans_count[s]]   = maskq;
                ++trans_count[s];
            }
        }
    }

    initialized = true;
}

/* ---------- Sequence retrieval ---------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n < 1 */

    if (!initialized) init();

    /* dp[state] holds the number of ways for the processed rows
       where current row pattern and unsatisfied mask correspond to the state.
       state = pattern * 16 + mask   (mask is 4 bits)   */
    static seq_t dp[1296];
    static seq_t newdp[1296];

    /* initialisation for row 1 (upper-left element must be 0) */
    for (int i = 0; i < 1296; ++i) dp[i] = 0;
    for (int p = 0; p < 81; ++p) {
        if (patterns[p][0] != 0) continue;          /* upper-left fixed to 0 */
        unsigned char mask = alone_unsat[p];
        dp[p * 16 + mask] += (seq_t)1;
    }

    /* iterate over remaining rows */
    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 1296; ++i) newdp[i] = 0;
        for (int s = 0; s < 1296; ++s) {
            seq_t cur = dp[s];
            if (!cur) continue;
            unsigned char cnt = trans_count[s];
            for (unsigned char t = 0; t < cnt; ++t) {
                unsigned char q = trans_pattern[s][t];
                unsigned char maskq = trans_mask[s][t];
                newdp[q * 16 + maskq] += cur;
            }
        }
        /* copy newdp to dp for next iteration */
        for (int i = 0; i < 1296; ++i) dp[i] = newdp[i];
    }

    /* final answer: all states with no unsatisfied cells in the last row */
    seq_t result = 0;
    for (int p = 0; p < 81; ++p) {
        result += dp[p * 16 + 0];
    }
    return result;
}
