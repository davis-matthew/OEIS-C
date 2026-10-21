/*
 * A230171  Number of n X 3 0..2 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value 2-x(i,j).
 * https://oeis.org/A230171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A230171: Number of n×3 arrays with values 0,1,2 such that every
 * element has at least one neighbor (horizontally, vertically,
 * diagonally or anti‑diagonally) with value 2‑x(i,j).  The implementation
 * uses a dynamic program over columns.  For each column configuration
 * (27 possibilities) we keep a 3‑bit mask of rows that are not yet
 * satisfied.  Transitions between configurations are pre‑computed once.
 *
 * Complexity:   O(n·27²·8)  ≤ 6·10⁷ elementary operations for n = 10 000.
 * Memory:       constant (a few hundred seq_t values).
 */
#include <stddef.h>
#include <stdint.h>

#define STATES 27
#define MASKS  8

seq_t a(idx_t n)
{
    /* pre‑computed data – initialise on first call */
    static int init_done = 0;
    static int val[STATES][3];
    static unsigned char init_unsat[STATES];
    static unsigned char sat_prev[STATES][STATES];
    static unsigned char unsat_next[STATES][STATES];

    if (!init_done) {
        /* enumerate all column configurations */
        for (int idx = 0; idx < STATES; ++idx) {
            int t = idx;
            for (int r = 0; r < 3; ++r) {
                val[idx][r] = t % 3;
                t /= 3;
            }
        }

        /* unsatisfied mask for the very first column (no left neighbour) */
        for (int idx = 0; idx < STATES; ++idx) {
            int mask = 0;
            for (int r = 0; r < 3; ++r) {
                int need = 2 - val[idx][r];
                int sat = 0;
                /* vertical neighbours within the same column */
                if (r > 0   && val[idx][r-1] == need) sat = 1;
                if (r < 2   && val[idx][r+1] == need) sat = 1;
                if (sat) mask |= (1 << r);
            }
            init_unsat[idx] = (~mask) & 7;          /* keep only 3 bits */
        }

        /* transitions between adjacent columns */
        for (int p = 0; p < STATES; ++p) {
            for (int c = 0; c < STATES; ++c) {
                int sat = 0;      /* rows of column p satisfied by column c   */
                int newmask = 0;  /* unsatisfied rows of column c after this step */
                for (int r = 0; r < 3; ++r) {
                    /* previous column row r */
                    int needPrev = 2 - val[p][r];
                    int satPrev = 0;
                    /* vertical neighbours in the same column p */
                    if (r > 0   && val[p][r-1] == needPrev) satPrev = 1;
                    if (r < 2   && val[p][r+1] == needPrev) satPrev = 1;
                    /* neighbours from column c */
                    if (val[c][r]     == needPrev) satPrev = 1;
                    if (r > 0   && val[c][r-1] == needPrev) satPrev = 1;
                    if (r < 2   && val[c][r+1] == needPrev) satPrev = 1;
                    if (satPrev) sat |= (1 << r);

                    /* current column row r: check satisfaction */
                    int needCur = 2 - val[c][r];
                    int satCur = 0;
                    /* vertical neighbours within column c */
                    if (r > 0   && val[c][r-1] == needCur) satCur = 1;
                    if (r < 2   && val[c][r+1] == needCur) satCur = 1;
                    /* left neighbours from column p */
                    if (val[p][r]     == needCur) satCur = 1;
                    if (r > 0   && val[p][r-1] == needCur) satCur = 1;
                    if (r < 2   && val[p][r+1] == needCur) satCur = 1;
                    if (!satCur) newmask |= (1 << r);
                }
                sat_prev[p][c]  = sat;
                unsat_next[c][p] = newmask;
            }
        }
        init_done = 1;
    }

    /* DP over columns */
    static seq_t dp[STATES][MASKS];
    static seq_t next[STATES][MASKS];

    for (int i = 0; i < STATES; ++i)
        for (int m = 0; m < MASKS; ++m)
            dp[i][m] = 0;

    for (int idx = 0; idx < STATES; ++idx)
        dp[idx][init_unsat[idx]] = (seq_t)1;

    for (idx_t col = 1; col < n; ++col) {
        for (int i = 0; i < STATES; ++i)
            for (int m = 0; m < MASKS; ++m)
                next[i][m] = 0;

        for (int p = 0; p < STATES; ++p) {
            for (int pm = 0; pm < MASKS; ++pm) {
                seq_t curVal = dp[p][pm];
                if (!curVal) continue;
                for (int c = 0; c < STATES; ++c) {
                    /* all unsatisfied rows of previous column must be satisfied */
                    if ((pm & (~sat_prev[p][c])) != 0) continue;
                    int nm = unsat_next[c][p];
                    next[c][nm] += curVal;
                }
            }
        }

        /* swap dp and next */
        for (int i = 0; i < STATES; ++i)
            for (int m = 0; m < MASKS; ++m)
                dp[i][m] = next[i][m];
    }

    /* sum results with no unsatisfied rows */
    seq_t result = 0;
    for (int idx = 0; idx < STATES; ++idx)
        result += dp[idx][0];

    return result;
}
