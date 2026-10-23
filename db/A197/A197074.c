/*
 * A197074  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 3,1,2,4,0 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over two-row states to count
   n×3 arrays with values 0..4 where each element x satisfies
   that the number of its horizontal and vertical neighbors equal
   to 3,1,2,4,0 for x = 0,1,2,3,4 (OEIS A197074).. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Constants for the problem */
#define ROWS      3
#define V         5
#define PAT   (V*V*V)          /* 5^3 = 125 possible rows */
#define STATES (PAT*PAT)       /* 125^2 = 15625 pair states */
#define MAX_TRANS (PAT)        /* maximum outgoing transitions per state */

/* Target neighbor values for each cell value */
static const int target[V] = {3, 1, 2, 4, 0};

/* Pre‑computed data structures ------------------------------------------------*/
static int pattern[PAT][ROWS];                /* pattern[i] = array of 3 cell values */
static int nxt[STATES][MAX_TRANS];           /* nxt[state][k] = next row index */
static unsigned char nxt_cnt[STATES];         /* number of transitions for state */
static bool prepared = false;

/* ----------------------------------------------------------------------------- */
/* Helper: check if a given row satisfies its neighbour constraints.  The
   parameters upIdx, curIdx, downIdx are indexes into 'pattern' or -1 meaning
   no neighbour.  Returns true if row 'curIdx' is valid. */
static bool rowValidIdx(int upIdx, int curIdx, int downIdx)
{
    const int *up   = (upIdx >= 0) ? pattern[upIdx] : NULL;
    const int *cur  = pattern[curIdx];
    const int *down = (downIdx >= 0) ? pattern[downIdx] : NULL;

    for (int j = 0; j < ROWS; ++j) {
        int v      = cur[j];
        int want   = target[v];
        int have   = 0;

        if (j > 0 && cur[j-1] == want) ++have;      /* left */
        if (j < ROWS-1 && cur[j+1] == want) ++have; /* right */
        if (up  && up[j]   == want) ++have;          /* up   */
        if (down && down[j]== want) ++have;          /* down */

        if (have != v) return false;
    }
    return true;
}

/* ----------------------------------------------------------------------------- */
/* Prepare static tables only once.  Called at the beginning of the first call. */
static void prepare_tables(void)
{
    if (prepared) return;

    /* Enumerate all possible 3‑cell rows (base‑5 representation) */
    for (int idx = 0; idx < PAT; ++idx) {
        pattern[idx][0] = idx / 25;
        pattern[idx][1] = (idx / 5) % 5;
        pattern[idx][2] = idx % 5;
    }

    /* Build transition lists: for each pair (prevprev, prev) store all
       possible curr rows that make prev satisfy its constraints.    */
    for (int upIdx = 0; upIdx < PAT; ++upIdx) {
        for (int prevIdx = 0; prevIdx < PAT; ++prevIdx) {
            int state   = upIdx * PAT + prevIdx;
            unsigned char cnt = 0;
            for (int currIdx = 0; currIdx < PAT; ++currIdx) {
                if (rowValidIdx(upIdx, prevIdx, currIdx)) {
                    nxt[state][cnt++] = currIdx;
                }
            }
            nxt_cnt[state] = cnt;
        }
    }

    prepared = true;
}

/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure tables are ready.  May be called repeatedly. */
    prepare_tables();

    /* DP arrays */
    static seq_t dp_cur[STATES];
    static seq_t dp_next[STATES];
    seq_t zero = 0;

    /* ---- Handle the trivial case n = 1 --------------------------------------- */
    if (n == 1) {
        seq_t ans = 0;
        for (int row = 0; row < PAT; ++row) {
            if (rowValidIdx(-1, row, -1))
                ans += 1;
        }
        return ans;
    }

    /* ---- Initialise DP for the first two rows -------------------------------- */
    for (int i = 0; i < STATES; ++i) dp_cur[i] = zero;
    for (int row1 = 0; row1 < PAT; ++row1) {
        for (int row2 = 0; row2 < PAT; ++row2) {
            if (rowValidIdx(-1, row1, row2)) {
                int state = row1 * PAT + row2;
                dp_cur[state] += 1;
            }
        }
    }

    /* ---- Process rows 3 .. n --------------------------------------------- */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i) dp_next[i] = zero;

        for (int state = 0; state < STATES; ++state) {
            seq_t cnt = dp_cur[state];
            if (cnt == 0) continue;
            int prevPrev = state / PAT;
            int prev     = state % PAT;

            unsigned char tcnt = nxt_cnt[state];
            for (unsigned char k = 0; k < tcnt; ++k) {
                int curr     = nxt[state][k];
                int nextState = prev * PAT + curr;
                dp_next[nextState] += cnt;
            }
        }
        /* Swap current and next DP buffers */
        for (int i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* ---- Final check for the last row ------------------------------------- */
    seq_t ans = 0;
    for (int state = 0; state < STATES; ++state) {
        seq_t cnt = dp_cur[state];
        if (cnt == 0) continue;

        int upRow    = state / PAT;
        int lastRow  = state % PAT;
        if (rowValidIdx(upRow, lastRow, -1)) {
            ans += cnt;
        }
    }
    return ans;
}
