/*
 * A202792  Number of n X 5 binary arrays with every one adjacent to another
 *          one horizontally, diagonally, antidiagonally or vertically.
 * https://oeis.org/A202792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count n×5 binary arrays with every 1 having at least one
 *            adjacent 1 (horizontally, vertically, or diagonally). The number
 *            is obtained by dynamic programming with a state consisting of
 *            (previous row pattern, unsatisfied bits in that row). See OEIS
 *            A202792
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const uint8_t MASK5 = 0x1F;                /* 5 low bits set */

/* ---------- Precomputed transition tables ---------- */
static int             g_trans_cnt[1024] = {0};
static unsigned short  g_trans[1024][32];
static uint8_t         g_intra_adj[32];
static uint8_t         g_neigh_prev[32];
static int             g_initialized = 0;

/* Initialise static tables only once */
static void init_tables(void)
{
    if (g_initialized) return;
    /* intra--adjacency mask for each pattern P:
       bits that have a horizontally adjacent 1 in the same row */
    for (int P = 0; P < 32; P++) {
        uint8_t left  = (P << 1) & MASK5;
        uint8_t right = (P >> 1);
        g_intra_adj[P] = (left & P) | (right & P);               /* subset of P */
    }
    /* neighbor mask for each pattern P from the previous row:
       bits that can satisfy a current cell (vertical or diagonal) */
    for (int P = 0; P < 32; P++) {
        uint8_t v       = P & MASK5;
        uint8_t diag_l  = (P >> 1) & MASK5;
        uint8_t diag_r  = (P << 1) & MASK5;
        g_neigh_prev[P] = v | diag_l | diag_r;                   /* subset of 0..4 */
    }

    /* Build transition lists:  (prevP, prevU)  -->  (currP, currU) */
    for (int prevP = 0; prevP < 32; prevP++) {
        for (int prevU = 0; prevU < 32; prevU++) {
            int prevIdx = (prevP << 5) | prevU;
            /* A state is invalid if unsatisfied bits are not a subset of pattern */
            if (prevU & (~prevP)) continue;
            int trans_cnt = 0;
            uint8_t neighPrevMask = g_neigh_prev[prevP];
            for (int currP = 0; currP < 32; currP++) {
                /* For each unsatisfied bit in prev row, verify a neighbor in curr row */
                uint8_t currAdjMask = ((currP << 1) | currP | (currP >> 1)) & MASK5;
                if ( (prevU & (~currAdjMask)) != 0) continue;
                /* Unatisfied bits in current row after considering neighbors from prev */
                uint8_t intraCurr = g_intra_adj[currP];
                uint8_t satisfied = intraCurr | neighPrevMask;
                uint8_t currU = currP & (~satisfied) & MASK5;
                /* Form next state index */
                int nextIdx = (currP << 5) | currU;
                g_trans[prevIdx][trans_cnt++] = (unsigned short)nextIdx;
            }
            g_trans_cnt[prevIdx] = trans_cnt;
        }
    }
    g_initialized = 1;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    init_tables();                     /* ensure tables exist */

    /* DP vectors of length 1024 */
    static seq_t dp_cur[1024];
    static seq_t dp_next[1024];
    /* zero current DP vector */
    for (int i = 0; i < 1024; i++) dp_cur[i] = 0;

    /* base case: first row */
    for (int P = 0; P < 32; P++) {
        uint8_t uns = P & (~g_intra_adj[P]) & MASK5;      /* bits needing below neighbor */
        int idx = (P << 5) | uns;
        dp_cur[idx] += (seq_t)1;
    }

    if (n == 1) {               /* finish after first row */
        seq_t result = 0;
        for (int P = 0; P < 32; P++) {
            int idx = (P << 5);   /* unsatisfied bits must be 0 */
            result += dp_cur[idx];
        }
        return result;
    }

    /* DP for rows 2 .. n */
    for (idx_t row = 2; row <= n; row++) {
        for (int i = 0; i < 1024; i++) dp_next[i] = 0;
        for (int prevIdx = 0; prevIdx < 1024; prevIdx++) {
            if (dp_cur[prevIdx] == 0) continue;
            seq_t val = dp_cur[prevIdx];
            int cnt = g_trans_cnt[prevIdx];
            for (int t = 0; t < cnt; t++) {
                int nextIdx = g_trans[prevIdx][t];
                dp_next[nextIdx] += val;
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < 1024; i++) dp_cur[i] = dp_next[i];
    }

    /* sum over states with no unsatisfied bits in last row */
    seq_t result = 0;
    for (int P = 0; P < 32; P++) {
        int idx = (P << 5);            /* unsatisfied bits = 0 */
        result += dp_cur[idx];
    }
    return result;
}
