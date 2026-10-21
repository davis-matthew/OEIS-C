/*
 * A230677  Number of 3Xn 0..2 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value 2-x(i,j).
 * https://oeis.org/A230677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP with transfer‑matrix for 3×n arrays over {0,1,2} with the
 *            neighbour condition. The state consists of a column (27
 *            possibilities) and a 3‑bit mask indicating which cells still
 *            need a complementary neighbour on the **right** side.
 *            Transitions are precomputed in trans_to[STATES][VALS] (mask =
 *            -1...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include guard for required types */
#include <stdbool.h>

#define VALS   27   /* 3³ possible columns */
#define MASKS  8    /* 3 bit deficit mask */
#define STATES (VALS*MASKS)

static int trans_to[STATES][VALS];
static int trans_cnt[STATES];
static bool initialized = false;

/* precompute transition table once */
static void init_transition(void) {
    if (initialized)
        return;
    initialized = true;

    /* column values in base‑3 representation: col[r] = value of row r (0‑based) */
    int col[VALS][3];
    for (int idx = 0; idx < VALS; ++idx) {
        int x = idx;
        for (int r = 0; r < 3; ++r) {
            col[idx][r] = x % 3;
            x /= 3;
        }
    }

    for (int src = 0; src < STATES; ++src) {
        int c1   = src / MASKS;
        int mask = src % MASKS;
        int cnt  = 0;

        for (int c2 = 0; c2 < VALS; ++c2) {
            /* 1. Check that all cells of the left column are satisfied by right side */
            bool ok = true;
            for (int i = 0; i < 3 && ok; ++i) {
                if (mask & (1 << i)) {
                    int comp = 2 - col[c1][i];
                    bool sat = false;
                    /* horizontal neighbour */
                    if (col[c2][i] == comp)
                        sat = true;
                    /* diagonal up */
                    if (i > 0 && col[c2][i - 1] == comp)
                        sat = true;
                    /* diagonal down */
                    if (i < 2 && col[c2][i + 1] == comp)
                        sat = true;
                    if (!sat)
                        ok = false;
                }
            }
            if (!ok)
                continue;   /* transition invalid */

            /* 2. Compute deficit mask for the right column (to be satisfied by the future) */
            int mask2 = 0;
            for (int j = 0; j < 3; ++j) {
                int comp = 2 - col[c2][j];
                bool sat = false;
                /* left horizontal neighbour */
                if (col[c1][j] == comp)
                    sat = true;
                /* left upper diagonal */
                if (j > 0 && col[c1][j - 1] == comp)
                    sat = true;
                /* left lower diagonal */
                if (j < 2 && col[c1][j + 1] == comp)
                    sat = true;
                if (!sat)
                    mask2 |= (1 << j);
            }

            int dest = c2 * MASKS + mask2;
            trans_to[src][cnt++] = dest;
        }
        trans_cnt[src] = cnt;
    }
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    init_transition();

    /* DP arrays for current and next column states */
    static seq_t curr[STATES];
    static seq_t next[STATES];

    /* clear current array */
    for (int i = 0; i < STATES; ++i)
        curr[i] = (seq_t)0;

    /* initial states: first column must rely on the right side */
    if (n >= 1) {
        for (int c = 0; c < VALS; ++c) {
            int id = c * MASKS + 7;          /* mask 111b */
            curr[id] += (seq_t)1;
        }
    }

    /* process subsequent columns */
    for (idx_t col = 2; col <= n; ++col) {
        /* reset next array */
        for (int i = 0; i < STATES; ++i)
            next[i] = (seq_t)0;

        for (int src = 0; src < STATES; ++src) {
            seq_t ways = curr[src];
            if (ways == (seq_t)0)
                continue;
            int cnt = trans_cnt[src];
            for (int k = 0; k < cnt; ++k) {
                int dest = trans_to[src][k];
                next[dest] += ways;
            }
        }
        /* move next to curr for next iteration */
        for (int i = 0; i < STATES; ++i)
            curr[i] = next[i];
    }

    /* after all columns, only states with no outstanding deficits are valid */
    seq_t result = (seq_t)0;
    for (int c = 0; c < VALS; ++c) {
        int id = c * MASKS;                /* mask 000b */
        result += curr[id];
    }
    return result;
}
