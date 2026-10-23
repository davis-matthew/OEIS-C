/*
 * A189645  Number of n X 4 array permutations with each element moving zero
 *          or one space horizontally, diagonally or antidiagonally.
 * https://oeis.org/A189645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 array permutations with each element moving
 *            zero or one space horizontally, diagonally or antidiagonally.
 *            Implementation via state‑transition dynamic programming over
 *            rows. The state is an 8‑bit mask: bits 0–3 are unmatched right
 *            cells in row r‑1, bits 4–7 are unmatched right...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define STATES 256

static int initialized = 0;
static seq_t transGen[STATES][STATES];
static seq_t transLast[STATES][STATES];

/* Recursive generator of transitions.
   cur      : current state index (0..255)
   col      : which left column (0..3) is being assigned
   lowMask  : bits of unmatched rights in row r‑1
   curMask  : bits of unmatched rights in row r
   highMask : bits of unmatched rights in row r+1 (the row being built)
   allowDown: 1 for a normal row (allow moves to r+1), 0 for the last row
   trans    : matrix to accumulate counts */
static void dfs_transition(int cur, int col, int lowMask, int curMask,
                           int highMask, int allowDown,
                           seq_t (*trans)[STATES]) {
    if (col == 4) {
        if (lowMask == 0) {
            int nextState = (curMask & 0xF) | (highMask << 4);
            trans[cur][nextState] = trans[cur][nextState] + 1;
        }
        return;
    }
    int c = col, tgt;

    /* Move to row r‑1 (deltaRow = -1) */
    tgt = c - 1;
    if (tgt >= 0 && (lowMask & (1 << tgt)))
        dfs_transition(cur, col + 1, lowMask & ~(1 << tgt), curMask,
                       highMask, allowDown, trans);
    tgt = c + 1;
    if (tgt < 4 && (lowMask & (1 << tgt)))
        dfs_transition(cur, col + 1, lowMask & ~(1 << tgt), curMask,
                       highMask, allowDown, trans);

    /* Move to row r  (deltaRow = 0) */
    tgt = c - 1;
    if (tgt >= 0 && (curMask & (1 << tgt)))
        dfs_transition(cur, col + 1, lowMask, curMask & ~(1 << tgt),
                       highMask, allowDown, trans);
    if (curMask & (1 << c))
        dfs_transition(cur, col + 1, lowMask, curMask & ~(1 << c),
                       highMask, allowDown, trans);
    tgt = c + 1;
    if (tgt < 4 && (curMask & (1 << tgt)))
        dfs_transition(cur, col + 1, lowMask, curMask & ~(1 << tgt),
                       highMask, allowDown, trans);

    /* Move to row r+1 (deltaRow = +1) – only if allowed */
    if (allowDown) {
        tgt = c - 1;
        if (tgt >= 0 && (highMask & (1 << tgt)))
            dfs_transition(cur, col + 1, lowMask, curMask,
                           highMask & ~(1 << tgt), allowDown, trans);
        tgt = c + 1;
        if (tgt < 4 && (highMask & (1 << tgt)))
            dfs_transition(cur, col + 1, lowMask, curMask,
                           highMask & ~(1 << tgt), allowDown, trans);
    }
}

/* Build all transitions for either normal rows (allowDown=1) or
   the last row (allowDown=0). */
static void build_transitions(int allowDown, seq_t (*trans)[STATES]) {
    int i, j;
    for (i = 0; i < STATES; i++)
        for (j = 0; j < STATES; j++)
            trans[i][j] = 0;

    for (i = 0; i < STATES; i++) {
        int low  = i & 0xF;
        int next = (i >> 4) & 0xF;
        int high = allowDown ? 0xF : 0;
        dfs_transition(i, 0, low, next, high, allowDown, trans);
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        build_transitions(1, transGen);
        build_transitions(0, transLast);
        initialized = 1;
    }

    static seq_t dp[STATES];
    static seq_t dpNext[STATES];
    int i, j;
    for (i = 0; i < STATES; i++) dp[i] = 0;
    dp[(0xF << 4)] = 1;              /* initial state: row0 all matched */

    for (idx_t step = 1; step <= n; step++) {
        seq_t (*Trans)[STATES] = (step == n) ? transLast : transGen;
        for (i = 0; i < STATES; i++) dpNext[i] = 0;

        for (i = 0; i < STATES; i++) {
            seq_t curVal = dp[i];
            if (curVal == 0) continue;
            for (j = 0; j < STATES; j++) {
                seq_t ways = Trans[i][j];
                if (ways == 0) continue;
                dpNext[j] = dpNext[j] + curVal * ways;
            }
        }
        for (i = 0; i < STATES; i++) dp[i] = dpNext[i];
    }

    return dp[0];
}
